#include "Frames/CompressMultipleFrame.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include "stb_image_write.h"

CompressMultipleFrame::CompressMultipleFrame(const wxString& title) : BaseCompressFrame(title)
{ }

bool CompressMultipleFrame::OnInit()
{
	wxPanel* panel = new wxPanel();

	return true;
}