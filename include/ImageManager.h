#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include <fstream>

class ImageManager
{
private:

	const inline static std::filesystem::path project_directory = PROJECT_ROOT;
	const inline static std::string default_image_output_directory = "transformed_images";
	const inline static std::filesystem::path default_image_output_path = project_directory / default_image_output_directory;

public: 
	bool save_image_dat(std::string image_file_name, uint8_t number_of_color_channels, uint8_t number_of_clusters,
		uint8_t* cluster_positions, uint16_t width, uint16_t height, uint8_t* index_map);
	bool load_image_dat(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
		uint8_t& number_of_clusters, uint8_t*& cluster_positions, uint16_t& width, uint16_t& height, uint8_t*& index_map);
};