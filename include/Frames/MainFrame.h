#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
private:

	wxFrame* current_frame = nullptr;

public:

	MainFrame(const wxString& title);

	// event handlers
	void compress_single_image(wxCommandEvent&);
	void compress_multiple_images(wxCommandEvent&);
	void display_image(wxCommandEvent&);

	// edit the current frame to have certain display configs within this function
	void default_configs_for_current_frame();

	// this is for reshowing this main frame once the option frame is closed
	void OnCompressFrameClose(wxCloseEvent& event);

};