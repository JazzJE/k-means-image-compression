#include "KClusterAlgorithm.h"
#include "Constants.h"

KClusterAlgorithm::KClusterAlgorithm() : cluster_positions(nullptr), associated_cluster_indices(nullptr), current_number_of_clusters(0), 
saved_number_of_pixels(0)
{ }

KClusterAlgorithm::~KClusterAlgorithm()
{
	delete[] cluster_positions;
	delete[] associated_cluster_indices;
}

void KClusterAlgorithm::cluster_data_samples(uint8_t* image_data, uint32_t new_number_of_pixels, uint8_t new_number_of_clusters)
{	
	// only reallocate the number of associated cluster indices if there are a different number of pixels; this is to save time
	if (new_number_of_pixels != saved_number_of_pixels)
	{
		delete[] associated_cluster_indices;
		saved_number_of_pixels = new_number_of_pixels;
		associated_cluster_indices = new uint8_t[saved_number_of_pixels];
	}

	// only reallocate the clusters if there are a different number of them; again, to save time
	if (new_number_of_clusters != current_number_of_clusters)
	{
		delete[] cluster_positions;
		current_number_of_clusters = new_number_of_clusters;
		cluster_positions = new uint8_t[current_number_of_clusters * number_of_features];
	}

	// initialize each cluster to one of the samples positions using k-means++
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

}

// for each sample/pixel, find the closest cluster, and return if there any sample switched clusters
bool KClusterAlgorithm::assign_associated_cluster_indices(uint8_t* image_data)
{
	bool clusters_switched = false;

	for (int i = 0; i < saved_number_of_pixels * number_of_features; i++)
	{

	}

	return clusters_switched;
}

// find and set the average location of each cluster
void KClusterAlgorithm::reposition_clusters_to_average_location(uint8_t* image_data)
{

}

uint8_t* KClusterAlgorithm::get_cluster_positions()
{ return cluster_positions; }
uint8_t* KClusterAlgorithm::get_associated_cluster_indices()
{ return associated_cluster_indices; }