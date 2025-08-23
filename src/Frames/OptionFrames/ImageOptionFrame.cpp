#include "Frames/OptionFrames/ImageOptionFrame.h"
#include <wx/app.h>

ImageOptionFrame::ImageOptionFrame(wxFrame* main_frame, const wxString& title) :
	
	wxFrame(main_frame, wxID_ANY, title), 
	main_frame(main_frame)

{ this->Bind(wxEVT_CLOSE_WINDOW, &ImageOptionFrame::OnClose, this); }

// used to delete this current frame and then also return to the menu
void ImageOptionFrame::return_to_menu_option(wxCommandEvent& /*event*/)
{
	main_frame->Show(true);
	main_frame->Raise();
	main_frame->SetFocus();
	this->Destroy();
}

// used for exiting the app when any option frame is closed
void ImageOptionFrame::OnClose(wxCloseEvent& /*event*/)
{ wxTheApp->ExitMainLoop(); }