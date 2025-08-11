#include "Frames/CompressSingleFrame.h"
#include "utils.h"

CompressSingleFrame::CompressSingleFrame(const wxString& title) : ImageOptionFrame(title), displayed_height(0), 
displayed_number_of_color_channels(0), displayed_width(0), initial_image(nullptr), transformed_cluster_positions(nullptr),
transformed_image(nullptr), transformed_number_of_clusters(0), current_image_path("")
{ }

void CompressSingleFrame::generate_frame()
{
    reset_frame(this);

    wxPanel* button_panel = new wxPanel(this), *image_panel = new wxPanel(this);
    wxStaticBitmap* initial_image_map, *transformed_image_map;
    
    // load the initial image if it exists
    wxBitmap current_bitmap = wxNullBitmap;
    if (initial_image != nullptr)
        current_bitmap = wxBitmap(*initial_image);
    initial_image_map = new wxStaticBitmap(image_panel, wxID_ANY, current_bitmap, wxDefaultPosition, wxDefaultSize);

    // load the transformed image if it exists
    current_bitmap = wxNullBitmap;
    if (transformed_image != nullptr)
        current_bitmap = wxBitmap(*transformed_image);
    transformed_image_map = new wxStaticBitmap(image_panel, wxID_ANY, current_bitmap, wxDefaultPosition, wxDefaultSize);


    // Create button in button panel
    wxButton* open_image_file_button = new wxButton(button_panel, wxID_ANY, "Open .dat File");
    wxButton* save_transformed_image_button = new wxButton(button_panel, wxID_ANY, "Save Image");

    // Bind button click to handler function
    open_image_file_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::open_image_file_option, this);
    save_transformed_image_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::save_transformed_image_option, this);
    
    // Set up sizers for layout
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // Add button to button panel's sizer
    button_sizer->Add(open_image_file_button, 0, wxALL, 5);
    button_sizer->Add(save_transformed_image_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // Add the static bitmap to the image panel's sizer
    image_sizer->Add(initial_image_map, 1, wxEXPAND | wxALL, 5);
    image_sizer->Add(transformed_image_map, 1, wxALL | wxALIGN_CENTER, 5);
    image_panel->SetSizer(image_sizer);

    // Add panels to main sizer
    main_sizer->Add(button_panel, 0, wxEXPAND);
    main_sizer->Add(image_panel, 1, wxEXPAND);

    // Set the main sizer for the frame
    this->SetSizer(main_sizer);

    // Optionally fit the frame to contents
    this->Layout();
    this->Fit();
}

void CompressSingleFrame::open_image_file_option(wxCommandEvent& /*event*/)
{
    current_image_path = ImageManager::select_image_path_option().ToStdString();

    // delete the current image objects if they exist before loading new images
    if (initial_image)
    { delete initial_image; initial_image = nullptr; }
    if (transformed_image)
    { delete transformed_image; transformed_image = nullptr; }


    if (!current_image_path.empty())
    {
        initial_image = new wxImage(current_image_path.string());


    }

    generate_frame();
}

void CompressSingleFrame::save_transformed_image_option(wxCommandEvent& /*event*/)
{
    wxMessageBox("Saving ");
    initial_image = new wxImage(current_image_path.string());

    generate_frame();
}