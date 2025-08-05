#pragma once
#include <filesystem>
#include <wx/wx.h>

class BaseCompressFrame : public wxFrame
{
public:
	BaseCompressFrame(const wxString& title);

	bool create_image_file(std::filesystem::path image_file_path);
	bool save_image_file(std::filesystem::path image_file_path);

};