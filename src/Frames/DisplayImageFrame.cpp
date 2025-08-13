#include <filesystem>

#include "ImageManager.h"
#include "Frames/DisplayImageFrame.h"
#include "utils.h"

DisplayImageFrame::DisplayImageFrame(const wxString& title, wxFrame* main_frame) : ImageOptionFrame(title, main_frame), displayed_image(nullptr), displayed_cluster_positions(nullptr)
{ draw_frame(); }

DisplayImageFrame::~DisplayImageFrame()
{
    delete[] displayed_cluster_positions;
    delete displayed_image;
}

void DisplayImageFrame::draw_frame()
{
    reset_frame(this);
    wxPanel* button_panel = new wxPanel(this);
    wxScrolledWindow* image_scroller = new wxScrolledWindow(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL);
    image_scroller->SetScrollRate(10, 10); // scroll step in pixels
    wxStaticBitmap* displayed_image_map;

    // Load the image
    wxBitmap current_bitmap = wxNullBitmap;
    if (displayed_image)
        current_bitmap = wxBitmap(*displayed_image);
    displayed_image_map = new wxStaticBitmap(image_scroller, wxID_ANY, current_bitmap);

    // Create buttons in button panel
    wxButton* open_dat_file_button = new wxButton(button_panel, wxID_ANY, "Open .dat File");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");

    // Bind button click to handler functions
    open_dat_file_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::open_dat_file_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::return_to_menu_option, this);

    // Set up sizers for layout
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // buttons
    button_sizer->Add(open_dat_file_button, 0, wxALL, 5);
    button_sizer->Add(return_to_menu_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // images
    image_sizer->Add(displayed_image_map, 0, wxALL, 5);
    image_scroller->SetSizer(image_sizer);
    image_scroller->FitInside(); // makes sure scrollbars know the content size

    // layout
    main_sizer->Add(button_panel, 0, wxEXPAND);   // buttons above
    main_sizer->Add(image_scroller, 1, wxEXPAND); // images below

    // Set the main sizer for the frame
    this->SetSizer(main_sizer);
    this->Layout();
    this->Show(true);
}

void DisplayImageFrame::open_dat_file_option(wxCommandEvent& /*event*/)
{
    std::filesystem::path dat_file_path = ImageManager::select_dat_path_option().ToStdString();
    
    // delete the current displayed image object if it exists
    if (displayed_image)
    { delete displayed_image; displayed_image = nullptr; }

    if (!dat_file_path.empty())
        displayed_image = ImageManager::load_dat_image_file(dat_file_path, displayed_number_of_color_channels, 
            displayed_number_of_clusters, displayed_cluster_positions, displayed_width, displayed_height);

    draw_frame();
}