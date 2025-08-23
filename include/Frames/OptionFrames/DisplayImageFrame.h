#pragma once
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/event.h>
#include <wx/string.h>
#include <wx/scrolwin.h>
#include <wx/statbmp.h>
#include <cstdint>

#include "Frames/OptionFrames/ImageOptionFrame.h"
#include "CustomWidgets/ImageDisplayPanel.h"

class DisplayImageFrame : public ImageOptionFrame
{
private:

	wxImage* displayed_image;
	uint8_t* compressed_cluster_positions;
	std::filesystem::path current_image_path;
	uint8_t compressed_number_of_clusters;
	uint8_t displayed_number_of_color_channels;

	wxScrolledWindow* const image_window;
	wxStaticBitmap* const displayed_image_map;
	ImageDescriptionPanel* const image_description_panel;

	// method called when wanting to load a wximage to the screen
	void open_dat_file_option(wxCommandEvent&);

	// method for updating the size of the displayed image when the frame is resized

	
	// allow user to change the size of the image using ctrl + scroll

	void refresh_image_maps_and_descriptions(bool clear_description = false);

public:

	DisplayImageFrame(wxFrame* main_frame, const wxString& title);
	~DisplayImageFrame();

};