#pragma once
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <omp.h>
#include <filesystem>
#include "KClusterAlgorithm.h"
#include "Frames/ImageOptionFrame.h"

class CompressSingleFrame : public ImageOptionFrame
{
private:

	wxSpinCtrl* clusters_spinctrl;

	KClusterAlgorithm cluster_algorithm;

	uint8_t compressed_number_of_clusters;
	uint8_t* compressed_cluster_positions;
	uint8_t* compressed_index_map;

	// the program currently will only support 3 color channels
	const uint8_t displayed_number_of_color_channels;

	uint32_t displayed_width;
	uint32_t displayed_height;

	std::filesystem::path current_image_path;
	wxImage* initial_image;
	wxImage* compressed_image;

	void draw_frame() override;

	// method called when wanting to load an image and get its data
	void open_image_file_option(wxCommandEvent&);

	// method called to save the transformed image using a .dat file
	void save_transformed_image_option(wxCommandEvent&);

	// method for generating a new transformed image with k-means clustering
	void generate_compressed_image_option(wxCommandEvent&);

public:

	CompressSingleFrame(const wxString& title, wxFrame* main_frame);

};