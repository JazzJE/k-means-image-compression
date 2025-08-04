#include "KClusterAlgorithm.h"
#include <random>

KClusterAlgorithm::KClusterAlgorithm() : cluster_positions(nullptr), associated_cluster_indices(nullptr), current_number_of_clusters(0), 
current_number_of_pixels(0)
{ }

KClusterAlgorithm::~KClusterAlgorithm()
{
	delete[] cluster_positions;
	delete[] associated_cluster_indices;
}

void KClusterAlgorithm::cluster_data_samples(uint8_t* image_data, uint32_t new_number_of_pixels, uint8_t new_number_of_clusters)
{	
	// only reallocate the number of associated cluster indices if there are a different number of pixels; this is to save time
	if (new_number_of_pixels != current_number_of_pixels)
	{
		delete[] associated_cluster_indices;
		current_number_of_pixels = new_number_of_pixels;
		associated_cluster_indices = new uint8_t[current_number_of_pixels]();
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
void KClusterAlgorithm::initialize_clusters(uint8_t* image_data)
{
	// select a random pixel to serve as the first cluster's initial starting location
	std::mt19937 mt(std::random_device{}());

	uint32_t first_random_index = mt() % current_number_of_pixels;
	for (size_t f = 0; f < number_of_features; f++)
		cluster_positions[f] = image_data[first_random_index * number_of_features + f];

	for (size_t cluster_index = 0; cluster_index < current_number_of_clusters; cluster_index++)
	{
		
	}
}

// for each sample/pixel, find the closest cluster, and return if there any sample switched clusters
bool KClusterAlgorithm::assign_associated_cluster_indices(uint8_t* image_data)
{
	bool clusters_switched = false;

	// for every pixel sample
	for (size_t pixel_index = 0; pixel_index < current_number_of_pixels; pixel_index++)
	{
		size_t pixel_first_feature_index = pixel_index * number_of_features;

		// for checking if the associated cluster changed for this pixel
		size_t initial_cluster_index = associated_cluster_indices[pixel_index];

		// assume the minimum distance can be found with the first cluster
		associated_cluster_indices[pixel_index] = 0;
		size_t cluster_minimum_distance = 0;
		int initial_distance_component = 0;
		for (size_t f = 0; f < number_of_features; f++)
		{
			initial_distance_component = 
				static_cast<int>(cluster_positions[f]) - static_cast<int>(image_data[pixel_first_feature_index + f]);
			initial_distance_component *= initial_distance_component;
			cluster_minimum_distance += initial_distance_component;
		}

		// for every cluster after the first cluster, see if the distances to them are smaller
		for (size_t cluster_index = 1; cluster_index < current_number_of_clusters; cluster_index++)
		{
			size_t cluster_first_feature_index = cluster_index * number_of_features;
			size_t new_cluster_distance = 0;
			int distance_component = 0;
			
			for (size_t f = 0; f < number_of_features; f++)
			{
				distance_component = 
					static_cast<int>(cluster_positions[cluster_first_feature_index + f]) - 
					static_cast<int>(image_data[pixel_first_feature_index + f]);
				distance_component *= distance_component;
				new_cluster_distance += distance_component;
			}

			// check the distance from the pixel to the cluster and if distance is smaller, then assign it to that index
			if (new_cluster_distance < cluster_minimum_distance)
			{
				cluster_minimum_distance = new_cluster_distance;
				associated_cluster_indices[pixel_index] = cluster_index;
			}
		}

		// if the new cluster index is not equal to the initial cluster index for this pixel, then the pixel switched clusters
		if (associated_cluster_indices[pixel_index] != initial_cluster_index)
			clusters_switched = true;
	}

	return clusters_switched;
}

// find and set the average location of each cluster
void KClusterAlgorithm::reposition_clusters_to_average_location(uint8_t* image_data)
{
	double* running_totals_each_cluster = new double[current_number_of_clusters * number_of_features]();
	uint64_t* number_of_samples_each_cluster = new uint64_t[current_number_of_clusters]();

	for (size_t pixel_index = 0; pixel_index < current_number_of_pixels; pixel_index++)
	{
		size_t cluster_index = associated_cluster_indices[pixel_index];
		for (size_t f = 0; f < number_of_features; f++)
			running_totals_each_cluster[cluster_index * number_of_features + f] += image_data[pixel_index * number_of_features + f];
		number_of_samples_each_cluster[cluster_index] += 1;
	}

	for (size_t cluster_index = 0; cluster_index < current_number_of_clusters; cluster_index++)
		// check if the number of samples is not equal to 0 since a division by zero error would occur
		if (number_of_samples_each_cluster[cluster_index] > 0)
			for (size_t f = 0; f < number_of_features; f++)
				cluster_positions[cluster_index * number_of_features + f] = std::round
				(running_totals_each_cluster[cluster_index * number_of_features + f] / number_of_samples_each_cluster[cluster_index]);

	delete[] running_totals_each_cluster;
	delete[] number_of_samples_each_cluster;
}

uint8_t* KClusterAlgorithm::get_cluster_positions()
{ return cluster_positions; }
uint8_t* KClusterAlgorithm::get_associated_cluster_indices()
{ return associated_cluster_indices; }