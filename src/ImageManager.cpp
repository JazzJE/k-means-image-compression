#include "ImageManager.h"

bool ImageManager::save_image_as_dat(std::string image_file_name, uint8_t number_of_color_channels, uint8_t number_of_clusters,
	uint8_t* cluster_positions, uint16_t width, uint16_t height, uint8_t* index_map)
{
	try
	{
		std::fstream output_file(default_image_output_path / (image_file_name + ".dat"), std::ios::binary | std::ios::out | std::ios::trunc);

		output_file.write(reinterpret_cast<const char*>(&number_of_color_channels), sizeof(uint8_t));
		output_file.write(reinterpret_cast<const char*>(&number_of_clusters), sizeof(uint8_t));

		// insert every element inside of the k cluster array which has (k * number_of_color_channels) elements
		size_t number_of_elements = number_of_clusters * number_of_color_channels;
		for (size_t i = 0; i < number_of_elements; i++)
			output_file.write(reinterpret_cast<const char*>(cluster_positions + i), sizeof(uint8_t));

		output_file.write(reinterpret_cast<const char*>(&width), sizeof(uint16_t));
		output_file.write(reinterpret_cast<const char*>(&height), sizeof(uint16_t));

		// insert the index map
		number_of_elements = width * height;
		for (size_t i = 0; i < number_of_elements; i++)
			output_file.write(reinterpret_cast<const char*>(index_map + i), sizeof(uint8_t));

		output_file.close();

		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool ImageManager::load_image_as_dat(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
	uint8_t& number_of_clusters, uint8_t*& cluster_positions, uint16_t& width, uint16_t& height, uint8_t*& index_map)
{
	try
	{
		std::fstream input_file(loaded_file_path, std::ios::binary | std::ios::out);

		input_file.read(reinterpret_cast<char*>(&number_of_color_channels), sizeof(uint8_t));
		input_file.read(reinterpret_cast<char*>(&number_of_clusters), sizeof(uint8_t));

		size_t number_of_elements = number_of_clusters * number_of_color_channels;
		cluster_positions = new uint8_t[number_of_elements];
		for (size_t i = 0; i < number_of_elements; i++)
			input_file.read(reinterpret_cast<char*>(cluster_positions + i), sizeof(uint8_t));

		input_file.read(reinterpret_cast<char*>(&width), sizeof(uint16_t));
		input_file.read(reinterpret_cast<char*>(&height), sizeof(uint16_t));

		number_of_elements = width * height;
		index_map = new uint8_t[number_of_elements];
		for (size_t i = 0; i < number_of_elements; i++)
			input_file.read(reinterpret_cast<char*>(number_of_elements + i), sizeof(uint8_t));

		input_file.close();

		return true;
	}
	catch (...)
	{
		return false;
	}
}

std::filesystem::path ImageManager::get_default_image_output_path()
{ return default_image_output_path; }