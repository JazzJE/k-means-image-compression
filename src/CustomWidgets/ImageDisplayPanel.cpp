#include <wx/filename.h>
#include <cstring>

#include "CustomWidgets/ImageDisplayPanel.h"

ImageDisplayPanel::ImageDisplayPanel(wxWindow* parent, std::filesystem::path image_path, uint32_t image_width, uint32_t image_height, 
	uint8_t number_of_color_channels, uint8_t number_of_clusters, const uint8_t* cluster_positions) :
	
	current_image_path(image_path),
	displayed_number_of_color_channels(number_of_color_channels),
	compressed_number_of_clusters(number_of_clusters),
	compressed_cluster_positions(nullptr),

	wxPanel(parent, wxID_ANY), 
	image_description_panel(new ImageDescriptionPanel(this, image_path, image_width, image_height,
		number_of_color_channels, cluster_positions, number_of_clusters)), 
	image_panel(new wxPanel(this, wxID_ANY)),
	initial_image_map(new wxStaticBitmap(image_panel, wxID_ANY, wxNullBitmap)),
	compressed_image_map(new wxStaticBitmap(image_panel, wxID_ANY, wxNullBitmap)),
	compressed_image(nullptr),
	initial_image(nullptr)

{
	// if the cluster positions exist and the compressed number of clusters and the number of colo0r channels is not or less than 0,
	// then initialize with a dynamic array
	if (cluster_positions && compressed_number_of_clusters > 0 && number_of_color_channels > 0)
	{
		uint8_t* temp_cluster_positions = new uint8_t[number_of_color_channels * compressed_number_of_clusters];
		std::memcpy(temp_cluster_positions, cluster_positions, number_of_color_channels * compressed_number_of_clusters * sizeof(uint8_t));
		compressed_cluster_positions = temp_cluster_positions;
	}

	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

	// images
	image_sizer->Add(initial_image_map, 1, wxALL | wxEXPAND, 5);
	image_sizer->Add(compressed_image_map, 1, wxALL | wxEXPAND, 5);
	image_panel->SetSizer(image_sizer);
	image_panel->SetDoubleBuffered(true);

	main_sizer->Add(image_description_panel, 0, wxEXPAND); // description of image above the images
	main_sizer->Add(image_panel, 1, wxEXPAND); // images below

	this->SetSizer(main_sizer);
	this->Layout();
	this->Show(true);
}

ImageDisplayPanel::~ImageDisplayPanel()
{
	if (initial_image) { delete initial_image; initial_image = nullptr; }
	if (compressed_image) { delete compressed_image; compressed_image = nullptr; }
	if (compressed_cluster_positions) { delete[] compressed_cluster_positions; compressed_cluster_positions = nullptr; }
}

void ImageDisplayPanel::resize_images()
{
	// only resize if there is an initial image to display
	if (initial_image)
	{
		wxSize image_panel_dimensions = image_panel->GetSize();

		// each image should roughly take half of the panel's width and height
		wxSize max_image_sizes(image_panel_dimensions.GetWidth() / 2, image_panel_dimensions.GetHeight());

		uint32_t displayed_width = initial_image->GetWidth();
		uint32_t displayed_height = initial_image->GetHeight();

		double scale_x = static_cast<double>(max_image_sizes.GetWidth()) / displayed_width;
		double scale_y = static_cast<double>(max_image_sizes.GetHeight()) / displayed_height;
		double scale_factor = std::min(scale_x, scale_y);

		uint32_t new_image_width = static_cast<uint32_t>(std::round(displayed_width * scale_factor));
		uint32_t new_image_height = static_cast<uint32_t>(std::round(displayed_height * scale_factor));

		wxImage resized_initial_image = initial_image->Scale(new_image_width, new_image_height, wxIMAGE_QUALITY_HIGH);
		initial_image_map->SetBitmap(wxBitmap(resized_initial_image));

		// resize the compressed image map if it exists
		if (compressed_image)
		{
			wxImage resized_compressed_image = compressed_image->Scale(new_image_width, new_image_height, wxIMAGE_QUALITY_HIGH);
			compressed_image_map->SetBitmap(wxBitmap(resized_compressed_image));
		}

		this->Layout();
	}
}

void ImageDisplayPanel::refresh_image_maps_and_descriptions(bool clear_description)
{
	wxBitmap curr_map = wxNullBitmap;
	uint32_t displayed_width = 0;
	uint32_t displayed_height = 0;

	// add the initial image to the screen if it now exists
	if (initial_image)
	{
		curr_map = wxBitmap(*initial_image);
		displayed_width = initial_image->GetWidth();
		displayed_height = initial_image->GetHeight();
	}
	initial_image_map->SetBitmap(curr_map);
	initial_image_map->Refresh();
	
	curr_map = wxNullBitmap;

	// add the compressed image to the screen if it now exists
	if (compressed_image)
		curr_map = wxBitmap(*compressed_image);
	compressed_image_map->SetBitmap(curr_map);
	compressed_image_map->Refresh();

	// update image description panel with the compressed image data
	if (clear_description)
		image_description_panel->update_attribute_values(current_image_path, displayed_width, displayed_height,
			displayed_number_of_color_channels);
	else
		image_description_panel->update_attribute_values(current_image_path, displayed_width, displayed_height,
			displayed_number_of_color_channels, compressed_number_of_clusters, compressed_cluster_positions);

	// resize the images to fit into the current frame's dimensions
	resize_images();

	this->Layout();
}

// accessor methods
wxImage* ImageDisplayPanel::get_initial_image() const { return initial_image; }
wxImage* ImageDisplayPanel::get_compressed_image() const { return compressed_image; }
std::filesystem::path ImageDisplayPanel::get_current_image_path() const { return current_image_path; }
uint8_t ImageDisplayPanel::get_displayed_number_of_color_channels() const { return displayed_number_of_color_channels; }
uint8_t ImageDisplayPanel::get_compressed_number_of_clusters() const { return compressed_number_of_clusters; }
const uint8_t* ImageDisplayPanel::get_compressed_cluster_positions() const { return compressed_cluster_positions; }

// setter methods
bool ImageDisplayPanel::set_initial_image(const wxImage* new_initial_image)
{
	// if the image is nullptr, then do nothing
	if (!new_initial_image) return false;

	// if the initial image is changing, then delete the old one and the current compressed image
	if (initial_image) { delete initial_image; initial_image = nullptr; }
	if (compressed_image) { delete compressed_image; compressed_image = nullptr; }

	initial_image = new wxImage(*new_initial_image);

	return true;
}
bool ImageDisplayPanel::set_compressed_image(const wxImage* new_compressed_image)
{
	// if the image is nullptr, then do nothing
	if (!new_compressed_image) return false;

	if (compressed_image) { delete compressed_image; compressed_image = nullptr; }
	compressed_image = new wxImage(*new_compressed_image);

	return true;
}

bool ImageDisplayPanel::set_current_image_path(const std::filesystem::path& new_image_path)
{
	wxFileName file_name(new_image_path.string());
	if (new_image_path.empty() || !file_name.IsOk()) return false; // do not set empty path

	current_image_path = new_image_path;
	return true;
}

bool ImageDisplayPanel::set_displayed_number_of_color_channels(uint8_t new_number_of_color_channels)
{
	if (new_number_of_color_channels < 1 || new_number_of_color_channels > 4) return false; // do not set invalid number of color channels
	
	displayed_number_of_color_channels = new_number_of_color_channels;
	return true;
}

bool ImageDisplayPanel::set_compressed_number_of_clusters(uint8_t new_number_of_clusters)
{
	if (new_number_of_clusters < 1) return false; // do not set zero or less than number of clusters

	compressed_number_of_clusters = new_number_of_clusters;
	return true;
}

bool ImageDisplayPanel::set_compressed_cluster_positions(const uint8_t* new_cluster_positions, uint8_t new_number_of_clusters, uint8_t number_of_color_channels)
{
	if (!new_cluster_positions || new_number_of_clusters <= 0) return false; // do not set to nullptr
	
	if (compressed_cluster_positions) { delete[] compressed_cluster_positions; compressed_cluster_positions = nullptr; }
	
	uint8_t* temp_positions = new uint8_t[new_number_of_clusters * number_of_color_channels];
	std::memcpy(temp_positions, new_cluster_positions, new_number_of_clusters * number_of_color_channels * sizeof(uint8_t));
	compressed_cluster_positions = temp_positions;

	return true;
}