#include "KClusterAlgorithm.h"
#include <random>
#include <omp.h>

KClusterAlgorithm::KClusterAlgorithm() : cluster_positions(nullptr), index_map(nullptr), current_number_of_clusters(0), 
current_number_of_pixels(0)
{ }

KClusterAlgorithm::~KClusterAlgorithm()
{
	if (cluster_positions) { delete[] cluster_positions; cluster_positions = nullptr; }
	if (index_map) { delete[] index_map; index_map = nullptr; }
}

void KClusterAlgorithm::cluster_data_samples(const uint8_t* image_data, size_t new_number_of_pixels, uint8_t new_number_of_clusters)
{	
	// only reallocate the number of associated cluster indices if there are a different number of pixels; this is to save time
	if (new_number_of_pixels != current_number_of_pixels)
	{
		delete[] index_map;
		current_number_of_pixels = new_number_of_pixels;
		index_map = new uint8_t[current_number_of_pixels]();
	}

	// only reallocate the clusters if there are a different number of them; again, to save time
	if (new_number_of_clusters != current_number_of_clusters)
	{
		delete[] cluster_positions;
		current_number_of_clusters = new_number_of_clusters;
		cluster_positions = new uint8_t[current_number_of_clusters * number_of_features]();
	}

	// initialize each cluster to one of the samples' positions using k-means++
	initialize_clusters(image_data);
	
	// while the number of reassignments does not equal to 0
	bool cluster_switched = false;
	while (true)
	{
		// assign each sample to its closest cluster
		cluster_switched = assign_associated_cluster_indices(image_data);

		// end if no samples switched clusters
		if (!cluster_switched)
			break;

		// find the average position of the cluster associated indices and then update the clusters to that position
		reposition_clusters_to_average_location(image_data);
	}
}

// initialize the clusters to different locations using k-means++
void KClusterAlgorithm::initialize_clusters(const uint8_t* image_data)
{
	std::mt19937 mt(std::random_device{}());
	double* shortest_distances = new double[current_number_of_pixels]();
	std::uniform_int_distribution<size_t> pixel_dist(0, current_number_of_pixels - 1);

	// select a random pixel to serve as the first cluster's initial starting location
	size_t first_random_index = pixel_dist(mt);
	for (size_t f = 0; f < number_of_features; f++)
		cluster_positions[f] = image_data[first_random_index * number_of_features + f];

	// for every new cluster
	for (size_t processing_cluster_index = 1; processing_cluster_index < current_number_of_clusters; processing_cluster_index++)
	{
		// for every pixel, find the shortest distance to a cluster
		#pragma omp parallel for
		for (int pixel_index = 0; pixel_index < current_number_of_pixels; pixel_index++)
		{
			shortest_distances[pixel_index] = 0;
			size_t pixel_first_feature_index = pixel_index * number_of_features;
			int64_t initial_distance_component = 0;

			// assume the shortest distance is to the first cluster
			for (size_t f = 0; f < number_of_features; f++)
			{
				initial_distance_component =
					static_cast<int64_t>(cluster_positions[f]) - static_cast<int64_t>(image_data[pixel_first_feature_index + f]);
				initial_distance_component *= initial_distance_component;
				shortest_distances[pixel_index] += initial_distance_component;
			}

			// for every cluster up until this new cluster, check the distance from the pixel to the cluster and if distance is smaller, 
			// then assign that shorter distance to that index
			for (size_t current_cluster_index = 1; current_cluster_index < processing_cluster_index; current_cluster_index++)
			{
				size_t current_cluster_first_feature_index = current_cluster_index * number_of_features;
				uintmax_t new_cluster_distance = 0;
				int64_t distance_component = 0;

				for (size_t f = 0; f < number_of_features; f++)
				{
					distance_component =
						static_cast<int64_t>(cluster_positions[current_cluster_first_feature_index + f]) -
						static_cast<int64_t>(image_data[pixel_first_feature_index + f]);
					distance_component *= distance_component;
					new_cluster_distance += distance_component;
				}

				if (new_cluster_distance < shortest_distances[pixel_index])
					shortest_distances[pixel_index] = new_cluster_distance;
			}
		}

		// sum the distances to get their total
		double distance_total = 0;
		#pragma omp parallel for reduction(+:distance_total)
		for (int i = 0; i < current_number_of_pixels; i++)
			distance_total += shortest_distances[i];

		// transform the array to have the percentages using the total
		#pragma omp parallel for
		for (int i = 0; i < current_number_of_pixels; i++)
			shortest_distances[i] /= distance_total;

		// have a random value picked from 0 to 1, and then sum until the sum is greater than or equal to the random value
		// the place where the sum stops will be the initial position of the cluster
		std::uniform_real_distribution<> dis(0.0, 1.0);
		double random_value = dis(mt), running_sum = 0;
		size_t chosen_index = 0;
		while (true)
		{
			running_sum += shortest_distances[chosen_index];
			if (running_sum >= random_value || chosen_index + 1 >= current_number_of_pixels)
				break;
			chosen_index++;
		}

		size_t current_cluster_index_first_feature = processing_cluster_index * number_of_features;
		size_t chosen_index_first_feature = chosen_index * number_of_features;
		for (int f = 0; f < number_of_features; f++)
			cluster_positions[current_cluster_index_first_feature + f] = image_data[chosen_index_first_feature + f];
	}

	delete[] shortest_distances;
}

// for each sample/pixel, find the closest cluster, and return if there any sample switched clusters
bool KClusterAlgorithm::assign_associated_cluster_indices(const uint8_t* image_data)
{
	bool clusters_switched = false;

	// for every pixel sample
	#pragma omp parallel for reduction(||:clusters_switched)
	for (int pixel_index = 0; pixel_index < current_number_of_pixels; pixel_index++)
	{
		size_t pixel_first_feature_index = pixel_index * number_of_features;

		// for checking if the associated cluster changed for this pixel
		size_t initial_cluster_index = index_map[pixel_index];

		// assume the minimum distance can be found with the first cluster
		index_map[pixel_index] = 0;
		uintmax_t cluster_minimum_distance = 0;
		int64_t initial_distance_component = 0;
		for (size_t f = 0; f < number_of_features; f++)
		{
			initial_distance_component = 
				static_cast<int64_t>(cluster_positions[f]) - static_cast<int64_t>(image_data[pixel_first_feature_index + f]);
			initial_distance_component *= initial_distance_component;
			cluster_minimum_distance += initial_distance_component;
		}

		// for every cluster after the first cluster, see if the distances to them are smaller
		for (size_t cluster_index = 1; cluster_index < current_number_of_clusters; cluster_index++)
		{
			size_t cluster_first_feature_index = cluster_index * number_of_features;
			uintmax_t new_cluster_distance = 0;
			int64_t distance_component = 0;
			
			for (size_t f = 0; f < number_of_features; f++)
			{
				distance_component = 
					static_cast<int64_t>(cluster_positions[cluster_first_feature_index + f]) - 
					static_cast<int64_t>(image_data[pixel_first_feature_index + f]);
				distance_component *= distance_component;
				new_cluster_distance += distance_component;
			}

			// check the distance from the pixel to the cluster and if distance is smaller, then assign it to that index
			if (new_cluster_distance < cluster_minimum_distance)
			{
				cluster_minimum_distance = new_cluster_distance;
				index_map[pixel_index] = cluster_index;
			}
		}

		// if the new cluster index is not equal to the initial cluster index for this pixel, then the pixel switched clusters
		if (index_map[pixel_index] != initial_cluster_index)
			clusters_switched = true;
	}

	return clusters_switched;
}

// find and set the average location of each cluster
void KClusterAlgorithm::reposition_clusters_to_average_location(const uint8_t* image_data)
{
	double* running_totals_each_cluster = new double[current_number_of_clusters * number_of_features]();
	size_t* number_of_samples_each_cluster = new size_t[current_number_of_clusters]();
	std::mt19937 mt(std::random_device{}());
	std::uniform_int_distribution<size_t> pixel_dist(0, current_number_of_pixels - 1);

	for (size_t pixel_index = 0; pixel_index < current_number_of_pixels; pixel_index++)
	{
		size_t cluster_index = index_map[pixel_index];
		for (size_t f = 0; f < number_of_features; f++)
			running_totals_each_cluster[cluster_index * number_of_features + f] += image_data[pixel_index * number_of_features + f];
		number_of_samples_each_cluster[cluster_index] += 1;
	}

	for (size_t cluster_index = 0; cluster_index < current_number_of_clusters; cluster_index++)
	{
		size_t cluster_first_feature_index = cluster_index * number_of_features;
		// check if the number of samples is not equal to 0
		if (number_of_samples_each_cluster[cluster_index] > 0)
			for (size_t f = 0; f < number_of_features; f++)
				cluster_positions[cluster_first_feature_index + f] = std::round
				(running_totals_each_cluster[cluster_first_feature_index + f] / number_of_samples_each_cluster[cluster_index]);

		// reinit the cluster to a new location if the number of samples assigned to it is 0
		else
		{
			size_t random_first_feature_index = pixel_dist(mt) * number_of_features;
			for (size_t f = 0; f < number_of_features; f++)
				cluster_positions[cluster_first_feature_index + f] = image_data[random_first_feature_index + f];
		}
	}

	delete[] running_totals_each_cluster;
	delete[] number_of_samples_each_cluster;
}

uint8_t* KClusterAlgorithm::get_cluster_positions() const
{ return cluster_positions; }
uint8_t* KClusterAlgorithm::get_index_map() const
{ return index_map; }