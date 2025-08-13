#include "Frames/ImageOptionFrame.h"

ImageOptionFrame::ImageOptionFrame(const wxString& title, wxFrame* main_frame) : wxFrame(nullptr, wxID_ANY, title), 
main_frame(main_frame)
{ }

void ImageOptionFrame::return_to_menu_option(wxCommandEvent& /*event*/)
{
	main_frame->Show(true);
	main_frame->Raise();
	main_frame->SetFocus();
	this->Destroy();
}