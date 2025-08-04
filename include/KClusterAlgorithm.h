#include <cstdint>
#include "Constants.h"

class KClusterAlgorithm
{
private:
	
	const inline static uint8_t number_of_features = Constants::number_of_color_channels;

	uint8_t current_number_of_clusters;
	uint8_t* cluster_positions;

	uint32_t current_number_of_pixels;
	uint8_t* associated_cluster_indices;

	void initialize_clusters(uint8_t* image_data);
	bool assign_associated_cluster_indices(uint8_t* image_data);
	void reposition_clusters_to_average_location(uint8_t* image_data);

public:

	KClusterAlgorithm();
	~KClusterAlgorithm();

	// cluster the data samples and return the data samples 
	void cluster_data_samples(uint8_t* image_data, uint32_t new_number_of_pixels, uint8_t new_number_of_clusters);

	uint8_t* get_cluster_positions();
	uint8_t* get_associated_cluster_indices();
};