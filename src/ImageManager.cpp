#include "ImageManager.h"

// given variables for an image, save the image in a specific format; return true if this was done successfully, else return false
bool ImageManager::save_image_as_dat(std::string image_file_name, uint8_t number_of_color_channels, uint8_t number_of_clusters,
	uint8_t* cluster_positions, uint32_t width, uint32_t height, uint8_t* index_map)
{
	std::ofstream output_file(default_image_output_path / (image_file_name + ".dat"), std::ios::binary | std::ios::trunc);
	if (!output_file.is_open())
	{
		output_file.close();
		return false;
	}

	try
	{
		output_file.write(reinterpret_cast<const char*>(&number_of_color_channels), sizeof(uint8_t));
		output_file.write(reinterpret_cast<const char*>(&number_of_clusters), sizeof(uint8_t));

		// insert every element inside of the k cluster array which has (k * number_of_color_channels) elements
		size_t number_of_elements = number_of_clusters * number_of_color_channels;
		for (size_t i = 0; i < number_of_elements; i++)
			output_file.write(reinterpret_cast<const char*>(cluster_positions + i), sizeof(uint8_t));

		output_file.write(reinterpret_cast<const char*>(&width), sizeof(uint32_t));
		output_file.write(reinterpret_cast<const char*>(&height), sizeof(uint32_t));

		// insert the index map
		number_of_elements = width * height;
		for (size_t i = 0; i < number_of_elements; i++)
			output_file.write(reinterpret_cast<const char*>(index_map + i), sizeof(uint8_t));

		output_file.close();

		return true;
	}
	catch (...)
	{
		output_file.close();
		return false;
	}
}

// load the image and return it as a heap-allocated object; return nullptr if the image is invalid
wxImage* ImageManager::load_dat_image_file(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
	uint8_t& number_of_clusters, uint8_t*& cluster_positions, uint32_t& width, uint32_t& height)
{
	// the index map doesn't need to be accessed beyond being used for generating the 1d array needed for the wximage
	uint8_t* index_map;
	if (!parse_dat_file(loaded_file_path, number_of_color_channels, number_of_clusters, cluster_positions, width, height, index_map))
	{
		wxMessageBox("Failed to load .dat file! Please ensure that it is one generated from this program.",
			"Error",
			wxOK | wxICON_ERROR);
		delete[] cluster_positions;
		delete[] index_map;

		cluster_positions = nullptr;
		index_map = nullptr;

		return nullptr;
	}

	size_t number_of_pixels = width * height;
	uint8_t* pixel_data = (uint8_t*)malloc(number_of_pixels * number_of_color_channels);

	// if the pixel_data failed to load, then return nullptr
	if (!pixel_data)
	{
		wxMessageBox("Failed to load .dat file! There was not enough memory in the heap.",
			"Error",
			wxOK | wxICON_ERROR);
		delete[] cluster_positions;
		delete[] index_map;

		cluster_positions = nullptr;
		index_map = nullptr;

		return nullptr;
	}

	// assign each pixel its cluster values
	for (size_t p = 0; p < number_of_pixels; p++)
	{
		size_t pixel_first_feature_index = p * number_of_color_channels;
		size_t cluster_first_feature_index = index_map[p] * number_of_color_channels;
		for (size_t f = 0; f < number_of_color_channels; f++)
			pixel_data[pixel_first_feature_index + f] = cluster_positions[cluster_first_feature_index + f];
	}

	delete[] index_map;

	// the image will take ownership of the data
	return new wxImage(width, height, pixel_data, false);
}

// edit the variables passed into the function for further usage beyond it; return true if the dat file was edited successfully, but
// return false if it was done unsuccessfully
bool ImageManager::parse_dat_file(std::filesystem::path loaded_file_path, uint8_t& number_of_color_channels,
	uint8_t& number_of_clusters, uint8_t*& cluster_positions, uint32_t& width, uint32_t& height, uint8_t*& index_map)
{
	std::ifstream input_file(loaded_file_path, std::ios::binary);
	if (!input_file.is_open())
	{
		input_file.close();
		return false;
	}

	try
	{
		input_file.read(reinterpret_cast<char*>(&number_of_color_channels), sizeof(uint8_t));
		input_file.read(reinterpret_cast<char*>(&number_of_clusters), sizeof(uint8_t));

		size_t number_of_elements = number_of_clusters * number_of_color_channels;
		cluster_positions = new uint8_t[number_of_elements];
		for (size_t i = 0; i < number_of_elements; i++)
			input_file.read(reinterpret_cast<char*>(cluster_positions + i), sizeof(uint8_t));

		input_file.read(reinterpret_cast<char*>(&width), sizeof(uint32_t));
		input_file.read(reinterpret_cast<char*>(&height), sizeof(uint32_t));

		number_of_elements = width * height;
		index_map = new uint8_t[number_of_elements];
		for (size_t i = 0; i < number_of_elements; i++)
			input_file.read(reinterpret_cast<char*>(index_map + i), sizeof(uint8_t));

		input_file.close();

		return true;
	}
	catch (...)
	{
		input_file.close();
		return false;
	}
}

// general method for having user select an image
wxString ImageManager::select_image_path_option()
{
	wxFileDialog open_dialog(nullptr,
		"Open Image File",
		"",
		"",
		"Image files (*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tga)|*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tga|"
		"PNG files (*.png)|*.png|"
		"JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|"
		"BMP files (*.bmp)|*.bmp|"
		"GIF files (*.gif)|*.gif|"
		"TIFF files (*.tiff)|*.tiff|"
		"TGA files (*.tga)|*.tga",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (open_dialog.ShowModal() == wxID_CANCEL)
		return "";

	return open_dialog.GetPath();
}

// general method for having user select a .dat file generated from this program which will be found in the image manager's default
// image output path
wxString ImageManager::select_dat_path_option()
{
	wxFileDialog open_dialog(nullptr,
		"Open .dat Image File",
		default_image_output_path.c_str(),
		"",
		"DAT files (*.dat)|*.dat",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (open_dialog.ShowModal() == wxID_CANCEL)
		return "";

	return open_dialog.GetPath();
}