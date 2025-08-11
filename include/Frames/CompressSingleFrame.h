#pragma once
#include <wx/wx.h>
#include <filesystem>
#include "KClusterAlgorithm.h"
#include "Frames/ImageOptionFrame.h"

class CompressSingleFrame : public ImageOptionFrame
{
private:

	KClusterAlgorithm cluster_algorithm;

	uint8_t transformed_number_of_clusters;
	uint8_t* transformed_cluster_positions;

	uint8_t displayed_number_of_color_channels;
	uint32_t displayed_width;
	uint32_t displayed_height;

	std::filesystem::path current_image_path;
	wxImage* initial_image;
	wxImage* transformed_image;

	void generate_frame() override;

	// method called when wanting to load an image and get its data
	void open_image_file_option(wxCommandEvent&);

	// method called to save the transformed image using a .dat file
	void save_transformed_image_option(wxCommandEvent&);

public:

	CompressSingleFrame(const wxString& title);

};