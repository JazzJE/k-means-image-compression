#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/wrapsizer.h>

#include "Frames/OptionFrames/CompressMultipleFrame.h"
#include "ImageManager.h"

CompressMultipleFrame::CompressMultipleFrame(wxFrame* main_frame, const wxString& title) : 
    ImageOptionFrame(main_frame, title),
	scrolled_window(new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL)),
	scrolled_sizer(new wxBoxSizer(wxHORIZONTAL))
{ 
	wxPanel* button_panel = new wxPanel(this, wxID_ANY);
    
    wxButton* open_image_file_button = new wxButton(button_panel, wxID_ANY, "Open Multiple Image Files");
    wxButton* generate_compressed_images_button = new wxButton(button_panel, wxID_ANY, "Generate Compressed Image");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");

    wxStaticText* clusters_label = new wxStaticText(button_panel, wxID_ANY, "Number of colors:");

    open_image_file_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::open_image_file_option, this);
    generate_compressed_images_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::generate_compressed_images_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::return_to_menu_option, this);

    // bind resize event to resize the images in the panel
    this->Bind(wxEVT_SIZE, &CompressMultipleFrame::OnResize, this);
    this->Bind(wxEVT_ICONIZE, &CompressMultipleFrame::OnMinimize, this);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

    // images
    /*
    image_sizer->Add(initial_image_map, 1, wxALL | wxEXPAND, 5);
    image_sizer->Add(compressed_image_map, 1, wxALL | wxEXPAND, 5);
    image_panel->SetSizer(image_sizer);
    image_panel->SetDoubleBuffered(true);

    // layout
    main_sizer->Add(button_panel, 0, wxEXPAND);   // buttons above
    main_sizer->Add(image_description_panel, 0, wxEXPAND); // description of image above the images
    main_sizer->Add(image_panel, 1, wxEXPAND); // images below

    */
    this->SetSizer(main_sizer);
    this->Layout();
    this->Show(true);
}

// called for when the user is resizing the frame
void CompressMultipleFrame::OnResize(wxSizeEvent& event)
{
    // if the user changes the state of the frame by clicking the restore window button, then resize the images
    static bool wasMaximized = true;
    if (IsMaximized() != wasMaximized)
    {
        wasMaximized = IsMaximized();
        CallAfter([this]() { return; });
    }
    // else, continue the resize normally; this is so that the image doesn't flicker
    else
        return;
    event.Skip();
}

void CompressMultipleFrame::OnMinimize(wxIconizeEvent& event)
{
    // resize images when restoring from minimized state
    if (!event.IsIconized())
        return;
    event.Skip();
}

void CompressMultipleFrame::generate_compressed_images_option(wxCommandEvent& /*event*/)
{
    return;
}

void CompressMultipleFrame::open_image_file_option(wxCommandEvent& /*event*/)
{
    wxArrayString new_image_paths = ImageManager::select_multiple_image_paths_option();
    // if the new image paths were valid
    if (!new_image_paths.empty())
    {
        /*
        // delete the current image objects if they exist before loading the new images
        if (initial_image) { delete initial_image; initial_image = nullptr; }
        if (compressed_image) { delete compressed_image; compressed_image = nullptr; }
        // load the first image as the initial image
        initial_image = new wxImage(new_image_paths[0].string());
        displayed_width = initial_image->GetWidth();
        displayed_height = initial_image->GetHeight();
        current_image_path = new_image_paths[0];
        ImageManager::remove_alpha_channels(initial_image);
        bool clear_description = true;
        refresh_image_maps_and_descriptions(clear_description);
        */
    }

    return;

}