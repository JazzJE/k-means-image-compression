#include <wx/wx.h>
#include <iostream>
#include <filesystem>
#include "Constants.h"
#include "Frames/MainFrame.h"
#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	// create the main option menu
	MainFrame* mainFrame = new MainFrame("K-Clustering Algorithm");
	mainFrame->SetClientSize(Constants::screen_width, Constants::screen_height);
	mainFrame->Center();
	mainFrame->Show();

	// must return true to make the program maintain the state of the program after init
	return true;
}