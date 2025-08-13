#pragma once
#include <wx/frame.h>
#include <wx/spinctrl.h>
#include <wx/event.h>
#include <wx/string.h>
#include <cstdint>
#include "ImageOptionFrame.h"

class DisplayImageFrame : public ImageOptionFrame
{
private:

	uint8_t displayed_number_of_color_channels;

	uint8_t displayed_number_of_clusters;
	uint8_t* displayed_cluster_positions;
	
	uint32_t displayed_width;
	uint32_t displayed_height;

	wxImage* displayed_image;

	void draw_frame() override;

	// method called when wanting to load a wximage to the screen
	void open_dat_file_option(wxCommandEvent&);

public:

	~DisplayImageFrame();
	DisplayImageFrame(const wxString& title, wxFrame* main_frame);

};