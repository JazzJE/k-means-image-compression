#pragma once
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/string.h>

class ImageOptionFrame : public wxFrame
{
protected:

	wxFrame* const main_frame;
	void return_to_menu_option(wxCommandEvent&);
	void OnClose(wxCloseEvent&);

public:

	ImageOptionFrame(wxFrame* main_frame, const wxString& title);

};