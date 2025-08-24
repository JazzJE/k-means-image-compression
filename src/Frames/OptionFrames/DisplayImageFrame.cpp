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
    displayed_number_of_color_channels(Constants::NUMBER_OF_COLOR_CHANNELS),

    image_window(new wxScrolledWindow(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL)),
    displayed_image_map(new wxStaticBitmap(image_window, wxID_ANY, wxNullBitmap)),
    image_description_panel(new ImageDescriptionPanel(this)),

    zoom_timer(new wxTimer(this, ID_ZoomTimer))
{
    image_window->SetScrollRate(10, 10); // scroll step in pixels

    // Create buttons in button panel
    wxPanel* button_panel = new wxPanel(this);
    wxButton* open_dat_file_button = new wxButton(button_panel, wxID_ANY, "Open .dat File");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");
	wxStaticText* instructions_label = new wxStaticText(button_panel, wxID_ANY, "Use Ctrl + Mouse Wheel to Zoom In/Out");

    // Bind button click to handler functions
    open_dat_file_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::open_dat_file_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::return_to_menu_option, this);

    this->Bind(wxEVT_MOUSEWHEEL, &DisplayImageFrame::resizing_displayed_image_option, this);
    image_window->Bind(wxEVT_MOUSEWHEEL, &DisplayImageFrame::resizing_displayed_image_option, this);

    this->Bind(wxEVT_SIZE, &DisplayImageFrame::OnResize, this);
    this->Bind(wxEVT_TIMER, &DisplayImageFrame::OnZoomTimer, this, ID_ZoomTimer);

    // Set up sizers for layout
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // buttons
    button_sizer->Add(open_dat_file_button, 0, wxALL, 5);
    button_sizer->Add(return_to_menu_button, 0, wxALL, 5);
	wxBoxSizer* vertical_centering_sizer1 = new wxBoxSizer(wxVERTICAL);
	vertical_centering_sizer1->AddStretchSpacer(1);
    vertical_centering_sizer1->Add(instructions_label, 0, wxALIGN_CENTER, 5);
    vertical_centering_sizer1->AddStretchSpacer(1);
	button_sizer->Add(vertical_centering_sizer1, 0, wxALL, 10);
    button_panel->SetSizer(button_sizer);

    // centering bitmap and adding image
    image_sizer->AddStretchSpacer(1);
    image_sizer->Add(displayed_image_map, 0, wxALIGN_CENTER_VERTICAL, 5);
    image_sizer->AddStretchSpacer(1); 
    wxBoxSizer* vertical_centering_sizer2 = new wxBoxSizer(wxVERTICAL);
    vertical_centering_sizer2->AddStretchSpacer(1);
    vertical_centering_sizer2->Add(image_sizer, 0, wxALIGN_CENTER_HORIZONTAL);
    vertical_centering_sizer2->AddStretchSpacer(1);

    image_window->SetSizer(vertical_centering_sizer2);
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
    if (zoom_timer) delete zoom_timer;
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
        
        zoom_factor = 1;
        resize_displayed_image();
    }
}

void DisplayImageFrame::OnResize(wxSizeEvent& event)
{
	// reset the zoom factor to 1 before resizing the image
    zoom_factor = 1;

    // if the user changes the state of the frame by clicking the restore window button, then resize the images after
    static bool wasMaximized = true;
    if (IsMaximized() != wasMaximized)
    {
        wasMaximized = IsMaximized();
        CallAfter([this]() { resize_displayed_image(); });
    }

    // else, continue the resize normally; this is so that the image doesn't flicker
    else
        resize_displayed_image();

    event.Skip();
}

void DisplayImageFrame::resizing_displayed_image_option(wxMouseEvent& event)
{
    if (event.ControlDown() && zoom_factor >= Constants::IMAGE_MIN_ZOOM_FACTOR && zoom_factor <= Constants::IMAGE_MAX_ZOOM_FACTOR)
    {
        int mouse_rotation = event.GetWheelRotation();

        if (mouse_rotation > 0)
            zoom_factor += 0.1;
        else if (mouse_rotation < 0)
            zoom_factor -= 0.1;

        // make sure the zoom_factor is in range to the limits
		zoom_factor = std::max(Constants::IMAGE_MIN_ZOOM_FACTOR, std::min(zoom_factor, Constants::IMAGE_MAX_ZOOM_FACTOR));
        
        zoom_timer->Stop();
        zoom_timer->StartOnce(50);
    }
    else
        event.Skip();
}

void DisplayImageFrame::OnZoomTimer(wxTimerEvent& /*event*/)
{ resize_displayed_image(); }

void DisplayImageFrame::resize_displayed_image()
{
    if (displayed_image)
    {
        uint32_t original_width = displayed_image->GetWidth();
        uint32_t original_height = displayed_image->GetHeight();
        wxSize max_image_sizes = image_window->GetSize();

        double scale_x = static_cast<double>(max_image_sizes.GetWidth()) / original_width;
        double scale_y = static_cast<double>(max_image_sizes.GetHeight()) / original_height;
        double scale_factor = std::min(scale_x, scale_y);

        uint32_t new_image_width = static_cast<uint32_t>(std::round(original_width * scale_factor * zoom_factor));
        uint32_t new_image_height = static_cast<uint32_t>(std::round(original_height * scale_factor * zoom_factor));

        // resize the image
        wxImage resized_image = displayed_image->Scale(new_image_width, new_image_height, wxIMAGE_QUALITY_HIGH);
        
        displayed_image_map->SetBitmap(wxBitmap(resized_image));
        displayed_image_map->Refresh(); // force repaint
        image_window->FitInside();

        this->Layout(); 
    }
}