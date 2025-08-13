#pragma once
#include <wx/wx.h>

#include "Frames/ImageOptionFrame.h"

class CompressMultipleFrame : public ImageOptionFrame
{
private:

	void draw_frame() override;

public:

	CompressMultipleFrame(const wxString& title, wxFrame* main_frame);

};