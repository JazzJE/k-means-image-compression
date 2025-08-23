#pragma once
#include <wx/panel.h>

#include <cstdint>
#include <filesystem>

#include "CustomWidgets/ListColorsPanel.h"
#include "Constants.h"

class ImageDescriptionPanel : public wxPanel
{
private:

	wxPanel* const path_panel; 
	wxPanel* const attributes_panel; 

	wxStaticText* const image_path_label;
	wxStaticText* const image_dimensions_label;
	wxStaticText* const number_of_color_channels_label; 
	wxStaticText* const number_of_clusters_label;

	ListColorsPanel* const list_colors_panel;

public:

	// the description panel will not display the colors if the cluster positions are not provided
	ImageDescriptionPanel(wxWindow* parent, std::filesystem::path image_path = "", uint32_t image_width = 0, uint32_t image_height = 0,
		uint8_t number_of_color_channels = Constants::number_of_color_channels, const uint8_t* cluster_positions = nullptr,
		uint8_t number_of_clusters = 0);

	// once again, description panel will not display colors if the cluster positions are not provided
	void update_attribute_values(std::filesystem::path new_image_path, uint32_t new_image_width, uint32_t new_image_height, 
		uint8_t new_number_of_color_channels, uint8_t new_number_of_clusters = 0, const uint8_t* new_cluster_positions = nullptr);

};