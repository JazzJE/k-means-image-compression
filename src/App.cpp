#include <wx/wx.h>
#include <iostream>
#include <filesystem>

#include "Constants.h"
#include "Frames/MainFrame.h"
#include "ImageManager.h"
#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	srand(time(0));

	wxInitAllImageHandlers();

	// this will be used for storing generated .dat files
	std::filesystem::create_directory(ImageManager::default_dat_output_path);
	
	// create the main option menu
	MainFrame* mainFrame = new MainFrame("K-Clustering Algorithm");
	mainFrame->Center();
	mainFrame->Show();

	// must return true to make the program maintain the state of the program after init
	return true;
}