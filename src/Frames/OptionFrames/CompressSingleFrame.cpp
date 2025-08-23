#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/wrapsizer.h>
#include <wx/busyinfo.h>

#include "Frames/OptionFrames/CompressSingleFrame.h"
#include "ImageManager.h"

CompressSingleFrame::CompressSingleFrame(wxFrame* main_frame, const wxString& title) :
    ImageOptionFrame(main_frame, title),

    // these should not be reallocated during frame's lifetime
	image_display_panel(new ImageDisplayPanel(this)),
	button_panel(new wxPanel(this, wxID_ANY)),
    clusters_spinctrl(new wxSpinCtrl(button_panel, wxID_ANY,
        wxString::Format("%d", 8),
        wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS,
        1, 255, 8)),
	current_index_map(nullptr)
{
    wxButton* open_image_file_button = new wxButton(button_panel, wxID_ANY, "Open Image File");
    wxButton* save_transformed_image_button = new wxButton(button_panel, wxID_ANY, "Save Transformed Image");
    wxButton* generate_compressed_image_button = new wxButton(button_panel, wxID_ANY, "Generate Compressed Image");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");

    wxStaticText* clusters_label = new wxStaticText(button_panel, wxID_ANY, "Number of colors:");

    open_image_file_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::open_image_file_option, this);
    save_transformed_image_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::save_transformed_image_option, this);
    generate_compressed_image_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::generate_compressed_image_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::return_to_menu_option, this);

	// bind resize event to resize the images in the panel
	this->Bind(wxEVT_SIZE, &CompressSingleFrame::OnResize, this);
    this->Bind(wxEVT_ICONIZE, &CompressSingleFrame::OnMinimize, this);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);

    // buttons
    button_sizer->Add(open_image_file_button, 0, wxALL, 5);
    button_sizer->Add(save_transformed_image_button, 0, wxALL, 5);
    button_sizer->AddSpacer(20);
    button_sizer->Add(generate_compressed_image_button, 0, wxALL, 5);
    button_sizer->AddSpacer(10);
    button_sizer->Add(clusters_label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    button_sizer->Add(clusters_spinctrl, 0, wxALL, 5);
    button_sizer->AddSpacer(20);
    button_sizer->Add(return_to_menu_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // layout
    main_sizer->Add(button_panel, 0, wxEXPAND);   // buttons above
    main_sizer->Add(image_display_panel, 1, wxEXPAND); // image panel

    this->SetSizer(main_sizer);
    this->Layout();
    this->Show(true);
}

// called for when the user is resizing the frame
void CompressSingleFrame::OnResize(wxSizeEvent& event)
{
    // if the user changes the state of the frame by clicking the restore window button, then resize the images
    static bool wasMaximized = true;
    if (IsMaximized() != wasMaximized) 
    {
        wasMaximized = IsMaximized();
        CallAfter([this]() { image_display_panel->resize_images(); });
    }
    // else, continue the resize normally; this is so that the image doesn't flicker
    else
        image_display_panel->resize_images();
    event.Skip();
}

// called for when the user minimizes the frame
void CompressSingleFrame::OnMinimize(wxIconizeEvent& event)
{
    // resize images when restoring from minimized state
    if (!event.IsIconized())
       image_display_panel->resize_images();
    event.Skip();
}

// prompt user to open an image file and load it into the frame
void CompressSingleFrame::open_image_file_option(wxCommandEvent& /*event*/)
{
    std::filesystem::path selected_image_path = ImageManager::select_image_path_option().ToStdString();

    // if the new image path was valid
    if (!selected_image_path.empty())
    {

        wxImage* new_initial_image = new wxImage(selected_image_path.string());
        std::filesystem::path new_image_path = selected_image_path;
        ImageManager::remove_alpha_channels(new_initial_image);

		// set the new image data to the image display panel
        if (!image_display_panel->set_initial_image(new_initial_image) || !image_display_panel->set_current_image_path(new_image_path))
        {
			wxMessageBox("Failed to load the image!", "Loading Warning", wxOK | wxICON_WARNING);
            delete new_initial_image;
            return;
        }
        delete new_initial_image;

        bool clear_description = true;
        image_display_panel->refresh_image_maps_and_descriptions(clear_description);
    }
}

// save the transformed image as a .dat file within the default output directory defined in ImageManager
void CompressSingleFrame::save_transformed_image_option(wxCommandEvent& /*event*/)
{
	wxImage* compressed_image = image_display_panel->get_compressed_image();

    // if there is a compressed image available to save, then we can save it
    if (compressed_image)
    {
		std::filesystem::path current_image_path = image_display_panel->get_current_image_path();

        wxFileName file_path(current_image_path.string());
        std::string image_file_name = file_path.GetName().ToStdString();

		uint8_t displayed_number_of_color_channels = image_display_panel->get_displayed_number_of_color_channels();
		uint8_t compressed_number_of_clusters = image_display_panel->get_compressed_number_of_clusters();
		const uint8_t* compressed_cluster_positions = image_display_panel->get_compressed_cluster_positions();
        uint32_t displayed_width = compressed_image->GetWidth();
        uint32_t displayed_height = compressed_image->GetHeight();

        if (!ImageManager::save_image_as_dat(image_file_name, displayed_number_of_color_channels, compressed_number_of_clusters,
            compressed_cluster_positions, displayed_width, displayed_height, current_index_map))
        {
            wxMessageBox("Failed to save the compressed image!", "Save Warning", wxOK | wxICON_WARNING);
            return;
        }

        wxMessageBox("Saved to " + (ImageManager::default_dat_output_path / (image_file_name + ".dat")).string(), "Saved", wxOK | wxICON_INFORMATION);
    }

    else
        wxMessageBox("There is no compressed image to save!", "Save Warning", wxOK | wxICON_WARNING);
}

// generate a compressed image using the k-means clustering algorithm
void CompressSingleFrame::generate_compressed_image_option(wxCommandEvent& /*event*/)
{   
	// if there is an initial image inside of the image display panel, then we can proceed with the compression
	wxImage* initial_image = image_display_panel->get_initial_image();
    if (initial_image)
    {
        uint32_t image_width = initial_image->GetWidth();
        uint32_t image_height = initial_image->GetHeight();

        uint8_t compressed_number_of_clusters = clusters_spinctrl->GetValue();
        size_t number_of_pixels = image_width * image_height;
        const uint8_t* const initial_image_data = initial_image->GetData();

        // cluster the data samples internally within the algorithm
        wxBusyInfo* busy = new wxBusyInfo("Loading, please wait...");
        cluster_algorithm.cluster_data_samples(initial_image_data, number_of_pixels, compressed_number_of_clusters);
        delete busy;

        // then get the resulting index map and cluster positions to construct the data of the transformed image
        const uint8_t* algorithm_cluster_positions = cluster_algorithm.get_cluster_positions();
        const uint8_t* algorithm_index_map = cluster_algorithm.get_index_map();
		uint8_t displayed_number_of_color_channels = image_display_panel->get_displayed_number_of_color_channels();

        wxImage* new_compressed_image = ImageManager::generate_compressed_image(displayed_number_of_color_channels, compressed_number_of_clusters,
            algorithm_cluster_positions, image_width, image_height, algorithm_index_map);

        if (new_compressed_image == nullptr)
        {
            wxMessageBox("Failed to compress image!", "Generation Warning", wxOK | wxICON_WARNING);
            return;
        }

		image_display_panel->set_compressed_image(new_compressed_image);
		image_display_panel->set_displayed_number_of_color_channels(displayed_number_of_color_channels);
		image_display_panel->set_compressed_number_of_clusters(compressed_number_of_clusters);
		image_display_panel->set_compressed_cluster_positions(algorithm_cluster_positions, compressed_number_of_clusters, displayed_number_of_color_channels);
        current_index_map = algorithm_index_map;

        delete new_compressed_image;

        image_display_panel->refresh_image_maps_and_descriptions();
    }
    else
        wxMessageBox("There is no initial image to transform!", "Generation Warning", wxOK | wxICON_WARNING);
}