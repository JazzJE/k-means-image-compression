#pragma once
#include <wx/wx.h>
#include "Frames/ImageOptionFrame.h"

class CompressMultipleFrame : public ImageOptionFrame
{
private:

	void generate_frame() override;

public:

	CompressMultipleFrame(const wxString& title);

};