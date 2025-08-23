#include <cstdint>
#include "Constants.h"

class KClusterAlgorithm
{
private:
	
	const inline static uint8_t number_of_features = Constants::number_of_color_channels;

	uint8_t current_number_of_clusters;
	uint8_t* cluster_positions;

	size_t current_number_of_pixels;
	uint8_t* index_map;

	void initialize_clusters(const uint8_t* image_data);
	bool assign_associated_cluster_indices(const uint8_t* image_data);
	void reposition_clusters_to_average_location(const uint8_t* image_data);

public:

	KClusterAlgorithm();
	~KClusterAlgorithm();

	// cluster the data samples and return the data samples 
	void cluster_data_samples(const uint8_t* image_data, size_t new_number_of_pixels, uint8_t new_number_of_clusters);

	uint8_t* get_cluster_positions() const;
	uint8_t* get_index_map() const;
};