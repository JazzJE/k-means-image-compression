#include "Frames/BaseCompressFrame.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION  
#include "stb_image_write.h"

BaseCompressFrame::BaseCompressFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{ }

bool BaseCompressFrame::save_image_file(std::filesystem::path image_file_path)
{
	stbi_write_png(image_file_path.string().c_str(), width, height, number_of_color_channels, 
		image_data, width * number_of_color_channels);
}