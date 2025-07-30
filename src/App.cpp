#include <wx/wx.h>
#include <iostream>
#include <filesystem>
#include "MainFrame.h"
#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	// get the number of colors each image should have for images we will skip personalization
	int default_number_of_colors = prompt_default_number_of_colors();

	if (std::filesystem::create_directory(initial_images_directory_path))
		std::cout << "Please place your images in the new " << initial_images_directory_name << " directory...";
	std::filesystem::create_directory(transformed_images_directory_path);

	// create a new k-means algorithm object, provided the number of colors channels in the images and the 

	// for each image file inside of the directory

	// load the image into the algorithm

	// if the image name does not begin with a tilde

		// save the image to the transformed images directory

	// else

		// while the user is unsatisfied with the result

			// prompt the user how many colors should the given image have

			// display the transformed result of the inputted image

			// ask the user if they are satisfied with the result

	// display a message saying that all images are transformed

	MainFrame* mainFrame = new MainFrame("K-Clustering Algorithm");
	mainFrame->Show();
	// must return true to make the program maintain the state of the program after init
	return true;
}

// helper method to have the user input the number of colors each image that we ignore personal selection will have
int App::prompt_default_number_of_colors()
{
	return 0;
}