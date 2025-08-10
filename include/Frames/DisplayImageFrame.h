#pragma once
#include <wx/wx.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <filesystem>
#include <cstdint>
#include "ImageOptionFrame.h"

class DisplayImageFrame : public ImageOptionFrame
{
private:

	uint8_t displayed_number_of_color_channels;
	uint8_t displayed_number_of_clusters;
	uint8_t* displayed_cluster_positions;
	uint8_t* displayed_index_map;
	uint16_t displayed_width;
	uint16_t displayed_height;

	wxImage* displayed_image;

	void generate_frame() override;

	// method called when wanting to load a wximage to the screen
	void open_dat_file_option(wxCommandEvent&);

public:

	~DisplayImageFrame();
	DisplayImageFrame(const wxString& title);

};