#pragma once
#include <filesystem>
#include <string>
#include <wx/wx.h>
#include "ImageManager.h"

class ImageOptionFrame : public wxFrame
{
private:

	// used to save images and load them within this frame
	ImageManager img_manager;
	
public:

	ImageOptionFrame(const wxString& title);
	
	virtual bool OnInit() = 0;

};