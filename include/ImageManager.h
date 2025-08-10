#pragma once
#include <wx/wx.h>
#include <wx/image.h>
#include <cstdint>
#include <string>
#include <filesystem>
#include <fstream>

namespace ImageManager
{

	const inline std::filesystem::path project_directory = PROJECT_ROOT;
	const inline std::string default_image_output_directory = "transformed_images";
	const inline std::filesystem::path default_image_output_path = project_directory / default_image_output_directory;

	// used for selecting an image to load into the program
	wxString select_image_path_option();

	// following used for loading and creating a .dat file image generated from this program
	wxImage* load_dat_image_file(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
		uint8_t& number_of_clusters, uint8_t*& cluster_positions, uint32_t& width, uint32_t& height);
	bool parse_dat_file(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
		uint8_t& number_of_clusters, uint8_t*& cluster_positions,  uint32_t& width, uint32_t& height, uint8_t*& index_map);
	wxString select_dat_path_option();
	

	bool save_image_as_dat(std::string image_file_name, uint8_t number_of_color_channels, uint8_t number_of_clusters,
		uint8_t* cluster_positions, uint32_t width, uint32_t height, uint8_t* index_map);
};