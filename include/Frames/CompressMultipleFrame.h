#pragma once
#include <wx/wx.h>
#include "BaseCompressFrame.h"

class CompressMultipleFrame : public BaseCompressFrame
{
public:

	CompressMultipleFrame(const wxString& title);

	bool OnInit();
};