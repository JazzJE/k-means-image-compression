#pragma once
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/string.h>

class MainFrame : public wxFrame
{
private:

	wxFrame* current_frame = nullptr;

	// event handlers
	void compress_single_image(wxCommandEvent&);
	void compress_multiple_images(wxCommandEvent&);
	void display_image(wxCommandEvent&);

	// edit the current frame to have certain display configs within this function
	void default_configs_for_current_frame();

public:

	MainFrame(const wxString& title);

};