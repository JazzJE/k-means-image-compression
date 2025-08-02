#include <cstdint>
#include "Constants.h"

class KClusterAlgorithm
{
private:
	
	const inline static uint8_t number_of_features = Constants::number_of_color_channels;

	uint8_t current_number_of_clusters;
	uint8_t** cluster_positions;

	uint32_t saved_number_of_samples;
	uint8_t* associated_cluster_indices;

	void deallocate_clusters();
	void reallocate_clusters();

public:

	KClusterAlgorithm();
	~KClusterAlgorithm();

	// cluster the data samples and return the data samples 
	uint8_t** cluster_data_samples(uint8_t** data_samples, uint32_t number_of_samples, uint8_t new_number_of_clusters);

};