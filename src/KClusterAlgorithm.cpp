#include "KClusterAlgorithm.h"
#include "Constants.h"

KClusterAlgorithm::KClusterAlgorithm() : cluster_positions(nullptr), associated_cluster_indices(nullptr), current_number_of_clusters(0), 
saved_number_of_samples(0)
{ }

KClusterAlgorithm::~KClusterAlgorithm()
{ 
	deallocate_clusters();
	delete[] associated_cluster_indices;
}

void KClusterAlgorithm::deallocate_clusters()
{
	if (cluster_positions != nullptr)
	{
		for (int i = 0; i < current_number_of_clusters; i++)
			delete[] cluster_positions[i];
		delete[] cluster_positions;
	}
}

void KClusterAlgorithm::reallocate_clusters()
{
	cluster_positions = new uint8_t*[current_number_of_clusters];
	for (int i = 0; i < current_number_of_clusters; i++)
		cluster_positions[i] = new uint8_t[number_of_features];
}


uint8_t** KClusterAlgorithm::cluster_data_samples(uint8_t** data_samples, uint32_t new_number_of_samples, uint8_t new_number_of_clusters)
{	
	// only reallocate the number of associated cluster indices if there are a different number of samples to save time
	if (new_number_of_samples != saved_number_of_samples)
	{
		delete[] associated_cluster_indices;
		saved_number_of_samples = new_number_of_samples;
		associated_cluster_indices = new uint8_t[saved_number_of_samples];
	}

	// only reallocate the clusters if there are a different number of them to save time
	if (new_number_of_clusters != current_number_of_clusters)
	{
		deallocate_clusters();
		current_number_of_clusters = new_number_of_clusters;
		reallocate_clusters();
	}

	// initialize each cluster to a one of the samples positions using k-means++

	
	// while the number of reassignments does not equal to 0

		// assign each sample to its closest cluster

		// find the average position


	// these will have corresponding samples indices point to the appropriate cluster positions
	uint8_t** transformed_samples = new uint8_t * [new_number_of_samples];
	for (int i = 0; i < new_number_of_samples; i++)
		transformed_samples[i] = new uint8_t[number_of_features];

	for (int i = 0; i < saved_number_of_samples; i++)
		transformed_samples[associated_cluster_indices[i]] = cluster_positions[i];

	return transformed_samples;
}