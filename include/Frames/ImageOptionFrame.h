#pragma once
#include <filesystem>
#include <string>
#include <wx/wx.h>
#include "ImageManager.h"

class ImageOptionFrame : public wxFrame
{
protected:

	// used to save images and load them within the frame
	ImageManager img_manager;
	
public:

	ImageOptionFrame(const wxString& title);

};