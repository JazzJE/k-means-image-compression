#include "Frames/CompressSingleFrame.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include "stb_image_write.h"

CompressSingleFrame::CompressSingleFrame(const wxString& title) : BaseCompressFrame(title)
{ }

bool CompressSingleFrame::OnInit()
{

	return true;
}