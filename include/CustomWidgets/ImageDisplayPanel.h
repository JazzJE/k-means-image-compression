#pragma once
#include <wx/panel.h>
#include <wx/gbsizer.h>
#include <filesystem>
#include <cstdint>

#include "ImageDescriptionPanel.h"
#include "Constants.h"

class ImageDisplayPanel : public wxPanel
{
private:

	wxImage* initial_image;
	wxImage* compressed_image; 
	std::filesystem::path current_image_path;
	uint8_t displayed_number_of_color_channels;
	uint8_t compressed_number_of_clusters;
	const uint8_t* compressed_cluster_positions;

	ImageDescriptionPanel* const image_description_panel; 
	wxPanel* const image_panel;
	wxStaticBitmap* const initial_image_map;
	wxStaticBitmap* const compressed_image_map;

public:

	ImageDisplayPanel(wxWindow* parent, std::filesystem::path image_path = "", uint32_t image_width = 0, uint32_t image_height = 0,
		uint8_t number_of_color_channels = Constants::number_of_color_channels, uint8_t number_of_clusters = 0, 
		const uint8_t* cluster_positions = nullptr);
	~ImageDisplayPanel();

	// update the image maps with the new images
	void refresh_image_maps_and_descriptions(bool clear_description = false);

	/// used for changing image sizes when the frame is resized or changes dimensions
	void resize_images();

	// accessor methods
	wxImage* get_initial_image() const;
	wxImage* get_compressed_image() const;
	std::filesystem::path get_current_image_path() const;
	uint8_t get_displayed_number_of_color_channels() const;
	uint8_t get_compressed_number_of_clusters() const;
	const uint8_t* get_compressed_cluster_positions() const;

	// used to update this panel's data
	bool set_initial_image(const wxImage* new_initial_image);
	bool set_compressed_image(const wxImage* new_compressed_image);
	bool  set_current_image_path(const std::filesystem::path& new_image_path);
	bool set_displayed_number_of_color_channels(uint8_t new_number_of_color_channels);
	bool set_compressed_number_of_clusters(uint8_t new_number_of_clusters);
	bool set_compressed_cluster_positions(const uint8_t* new_cluster_positions, uint8_t new_number_of_clusters, 
		uint8_t new_number_of_color_channels = Constants::number_of_color_channels);

};