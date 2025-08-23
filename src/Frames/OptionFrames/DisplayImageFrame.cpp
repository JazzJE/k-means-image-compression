#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/image.h>
#include <wx/scrolwin.h>
#include <filesystem>

#include "ImageManager.h"
#include "Frames/OptionFrames/DisplayImageFrame.h"
#include "Constants.h"

DisplayImageFrame::DisplayImageFrame(wxFrame* main_frame, const wxString& title) :
    
    ImageOptionFrame(main_frame, title),
    
    displayed_image(nullptr), 
    compressed_cluster_positions(nullptr),
    
    compressed_number_of_clusters(0),
    displayed_number_of_color_channels(Constants::number_of_color_channels),

    image_window(new wxScrolledWindow(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL)),
    displayed_image_map(new wxStaticBitmap(image_window, wxID_ANY, wxNullBitmap)),
    image_description_panel(new ImageDescriptionPanel(this))
{
    image_window->SetScrollRate(10, 10); // scroll step in pixels

    // Create buttons in button panel
    wxPanel* button_panel = new wxPanel(this);
    wxButton* open_dat_file_button = new wxButton(button_panel, wxID_ANY, "Open .dat File");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");

    // Bind button click to handler functions
    open_dat_file_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::open_dat_file_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::return_to_menu_option, this);

    // Set up sizers for layout
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // buttons
    button_sizer->Add(open_dat_file_button, 0, wxALL, 5);
    button_sizer->Add(return_to_menu_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // images
    image_sizer->Add(displayed_image_map, 0, wxALL, 5);
    image_window->SetSizer(image_sizer);
    image_window->FitInside(); // makes sure scrollbars know the content size

    // layout
    wxBoxSizer* main_window_sizer = new wxBoxSizer(wxVERTICAL);
    main_window_sizer->Add(button_panel, 0, wxEXPAND);   // buttons above
    main_window_sizer->Add(image_description_panel, 0, wxEXPAND);
    main_window_sizer->Add(image_window, 1, wxEXPAND); // images below

    // Set the main sizer for the frame
	this->SetSizer(main_window_sizer);
    this->Layout();
    this->Show(true);
}

DisplayImageFrame::~DisplayImageFrame()
{ 
	if (displayed_image) { delete displayed_image; displayed_image = nullptr; }
    if (compressed_cluster_positions) { delete[] compressed_cluster_positions; compressed_cluster_positions = nullptr; }
}

void DisplayImageFrame::refresh_image_maps_and_descriptions(bool clear_description)
{
    wxBitmap curr_map = wxNullBitmap;

    // add the initial image to the screen if it now exists
    if (displayed_image)
        curr_map = wxBitmap(*displayed_image);
    displayed_image_map->SetBitmap(curr_map);
    displayed_image_map->Refresh(); // Force repaint

    // update image description panel with the compressed image data
    if (clear_description)
        image_description_panel->update_attribute_values(current_image_path, displayed_image->GetWidth(), displayed_image->GetHeight(),
            displayed_number_of_color_channels);
    else
        image_description_panel->update_attribute_values(current_image_path, displayed_image->GetWidth(), displayed_image->GetHeight(),
            displayed_number_of_color_channels, compressed_number_of_clusters, compressed_cluster_positions);

    this->Layout();
}

void DisplayImageFrame::open_dat_file_option(wxCommandEvent& /*event*/)
{
    std::filesystem::path dat_file_path = ImageManager::select_dat_path_option().ToStdString();

    if (!dat_file_path.empty())
    {
        if (displayed_image) { delete displayed_image; displayed_image = nullptr; }
        if (compressed_cluster_positions) { delete[] compressed_cluster_positions; compressed_cluster_positions = nullptr; }

        // load .dat file passes by reference all parameters
        displayed_image = ImageManager::load_dat_file(dat_file_path, displayed_number_of_color_channels,
            compressed_number_of_clusters, compressed_cluster_positions);

        refresh_image_maps_and_descriptions();
    }
}