#pragma once
#include <wx/wx.h>
#include "BaseCompressFrame.h"

class CompressSingleFrame : public BaseCompressFrame
{
public:

	CompressSingleFrame(const wxString& title);

	bool OnInit();
};