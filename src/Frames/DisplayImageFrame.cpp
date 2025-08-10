#include "Frames/DisplayImageFrame.h"
#include "utils.h"

DisplayImageFrame::DisplayImageFrame(const wxString& title) : ImageOptionFrame(title), displayed_image(nullptr)
{
	generate_frame();
}

DisplayImageFrame::~DisplayImageFrame()
{
    delete[] displayed_cluster_positions;
    delete displayed_image;
}

void DisplayImageFrame::generate_frame()
{
	reset_frame(this);

	wxPanel* button_panel = new wxPanel(this), *image_panel = new wxPanel(this);

	wxStaticBitmap* displayed_image_map;

	if (displayed_image == nullptr)
		displayed_image_map = new wxStaticBitmap(
			image_panel,          // Parent window
			wxID_ANY,             // Control ID
			wxNullBitmap,         // Start with empty bitmap
			wxDefaultPosition,    // Position
			wxDefaultSize         // Size
		);
	else
		displayed_image_map = new wxStaticBitmap(
			image_panel,          // Parent window
			wxID_ANY,             // Control ID
			wxBitmap(*displayed_image),         // Start with empty bitmap
			wxDefaultPosition,    // Position
			wxDefaultSize         // Size
		);

    // Create button in button panel
    wxButton* open_dat_file_button = new wxButton(button_panel, wxID_ANY, "Open .dat File");

    // Bind button click to handler function
    open_dat_file_button->Bind(wxEVT_BUTTON, &DisplayImageFrame::open_dat_file_option, this);

    // Set up sizers for layout
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // Add button to button panel's sizer
    button_sizer->Add(open_dat_file_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // Add the static bitmap to the image panel's sizer
    image_sizer->Add(displayed_image_map, 1, wxEXPAND | wxALL, 5);
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

void DisplayImageFrame::open_dat_file_option(wxCommandEvent& /*event*/)
{
    std::filesystem::path dat_file_path = ImageManager::select_dat_path_option().ToStdString();
    displayed_image = ImageManager::load_dat_image_file(dat_file_path);

}