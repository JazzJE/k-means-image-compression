#include "CustomWidgets/ImageDescriptionPanel.h"
#include <wx/wrapsizer.h>

ImageDescriptionPanel::ImageDescriptionPanel(wxWindow* parent, std::filesystem::path image_path, uint32_t image_width, 
	uint32_t image_height, uint8_t number_of_color_channels, const uint8_t* cluster_positions, uint8_t number_of_clusters) :
	wxPanel(parent),

	path_panel(new wxPanel(this)),
	attributes_panel(new wxPanel(this)),

	image_path_label(new wxStaticText(path_panel, wxID_ANY, "")),
	image_dimensions_label(new wxStaticText(attributes_panel, wxID_ANY, "")),
	number_of_color_channels_label(new wxStaticText(attributes_panel, wxID_ANY, "")),
	number_of_clusters_label(new wxStaticText(attributes_panel, wxID_ANY, "")),

	list_colors_panel(new ListColorsPanel(this, number_of_color_channels, number_of_clusters, cluster_positions))
{
	// image path display
	wxWrapSizer* path_sizer = new wxWrapSizer(wxHORIZONTAL);
	path_sizer->Add(image_path_label, 0, wxALL, 5);
	path_panel->SetSizer(path_sizer);

	// general attributes display
	wxWrapSizer* attributes_sizer = new wxWrapSizer(wxHORIZONTAL);
		// add labels to the sizer
	attributes_sizer->Add(image_dimensions_label, 0, wxALL, 5);
	attributes_sizer->Add(number_of_color_channels_label, 0, wxALL, 5);
	attributes_sizer->Add(number_of_clusters_label, 0, wxALL, 5);
		// set sizer
	attributes_panel->SetSizer(attributes_sizer);

	// main panel sizer
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	main_sizer->Add(path_panel, 0, wxEXPAND | wxALL, 5);
	main_sizer->Add(attributes_panel, 0, wxEXPAND | wxALL, 5);
	main_sizer->Add(list_colors_panel, 1, wxEXPAND | wxALL, 5);
	this->SetSizer(main_sizer);

	update_attribute_values(image_path, image_width, image_height, number_of_color_channels, number_of_clusters, cluster_positions);
}

void ImageDescriptionPanel::update_attribute_values(std::filesystem::path new_image_path, uint32_t new_image_width, 
	uint32_t new_image_height, uint8_t new_number_of_color_channels, uint8_t new_number_of_clusters, 
	const uint8_t* new_cluster_positions)
{
	// path
	image_path_label->SetLabel("Image Path: " + new_image_path.string());

	// attributes
	image_dimensions_label->SetLabel(wxString::Format("Image Dimensions: %dx%d", new_image_width, new_image_height));
	number_of_color_channels_label->SetLabel(wxString::Format("Number of color channels: %d", new_number_of_color_channels));
	number_of_clusters_label->SetLabel(wxString::Format("Number of colors: %d", new_number_of_clusters));

	// list colors panel
	list_colors_panel->update_colors(new_number_of_color_channels, new_number_of_clusters, new_cluster_positions);
}