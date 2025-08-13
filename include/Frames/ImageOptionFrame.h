#pragma once
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/string.h>

class ImageOptionFrame : public wxFrame
{
protected:

	wxFrame* const main_frame;

	// every frame will have to have this method to reassemble itself given updated an state
	virtual void draw_frame() = 0;

	// used to delete this current frame and then also return to the menu
	void return_to_menu_option(wxCommandEvent&);
	
public:

	ImageOptionFrame(const wxString& title, wxFrame* main_frame);

};