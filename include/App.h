#pragma once
#include <wx/wx.h>
#include <filesystem>

class App : public wxApp
{
private:
	
	// directories where preprocessed and processed images will be placed
	const std::string initial_images_directory_name = "initial_images_directory";
	const std::string transformed_images_directory_name = "transformed_images_directory";
	const std::filesystem::path project_root = PROJECT_ROOT;
	const std::filesystem::path initial_images_directory_path = project_root / initial_images_directory_name;
	const std::filesystem::path transformed_images_directory_path = project_root / transformed_images_directory_name;

	// this will be the number of colors used for any images with a tilde behind their name
	int default_number_of_colors;

	// helper methods for initialization
	int prompt_default_number_of_colors();

public:

	bool OnInit();
};