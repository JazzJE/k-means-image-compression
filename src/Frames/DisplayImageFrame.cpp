#include "Frames/DisplayImageFrame.h"
#include "utils.h"

DisplayImageFrame::DisplayImageFrame(const wxString& title) : ImageOptionFrame(title), initial_image(nullptr), transformed_image(nullptr)
{
    wxString file_path = select_dat_path_option();

    // Check if file path is valid
    if (file_path.IsEmpty()) {
        wxLogError("No file selected");
        return;
    }

    // Create panel
    panel = new wxPanel(this);

    // Load image with error checking
    initial_image = new wxImage();
    if (!initial_image->LoadFile(file_path, wxBITMAP_TYPE_ANY)) {
        wxLogError("Failed to load image from: %s", file_path);
        delete initial_image;
        initial_image = nullptr;
        return;
    }

    // Verify image is valid
    if (!initial_image->IsOk()) {
        wxLogError("Invalid image data");
        delete initial_image;
        initial_image = nullptr;
        return;
    }

    // Create scrolled window
    scroll_window = new wxScrolledWindow(panel, wxID_ANY);
    scroll_window->SetScrollbars(20, 20, 50, 50);

    // Create static bitmap control
    initial_image_display = new wxStaticBitmap(scroll_window, wxID_ANY, wxNullBitmap);

    // Convert image to bitmap and display
    wxBitmap bitmap(*initial_image);
    initial_image_display->SetBitmap(bitmap);

    // IMPORTANT: Set up sizers for proper layout

    // Sizer for the scrolled window content
    wxBoxSizer* scroll_sizer = new wxBoxSizer(wxVERTICAL);
    scroll_sizer->Add(initial_image_display, 0, wxALL | wxALIGN_CENTER, 5);
    scroll_window->SetSizer(scroll_sizer);

    // Main sizer for the panel
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(scroll_window, 1, wxEXPAND | wxALL, 5);
    panel->SetSizer(main_sizer);

    // Update scroll window virtual size
    scroll_window->SetVirtualSize(initial_image->GetSize());

    // Force layout update
    scroll_window->FitInside();
    panel->Layout();
    Layout(); // Don't forget the frame layout!

    wxLogMessage("Image loaded successfully: %dx%d",
        initial_image->GetWidth(), initial_image->GetHeight());
}