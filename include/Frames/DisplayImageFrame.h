#pragma once
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/filename.h>
#include "ImageOptionFrame.h"

class DisplayImageFrame : public ImageOptionFrame
{
private:

	wxImage* initial_image;
	wxImage* transformed_image;

	wxStaticBitmap* initial_image_display;
	wxStaticBitmap* transformed_image_display;

	// make the window scrollable
	wxScrolledWindow* scroll_window;

	wxPanel* panel;

public:

	DisplayImageFrame(const wxString& title);

};