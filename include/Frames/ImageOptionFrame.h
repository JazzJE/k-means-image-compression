#pragma once
#include <filesystem>
#include <string>
#include <wx/wx.h>
#include <wx/image.h>
#include "ImageManager.h"

class ImageOptionFrame : public wxFrame
{
protected:

	virtual void generate_frame() = 0;
	
public:

	ImageOptionFrame(const wxString& title);

};