#pragma once
#include <wx/wx.h>
#include "Frames/ImageOptionFrame.h"

class CompressSingleFrame : public ImageOptionFrame
{
public:

	CompressSingleFrame(const wxString& title);

	bool OnInit();
};