#include <wx/stattext.h>
#include <wx/statbmp.h>
#include <wx/button.h>
#include <wx/wrapsizer.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/busyinfo.h>
#include <wx/msgdlg.h>
#include <wx/statline.h>
#include <string>
#include <cstring>

#include "Frames/OptionFrames/CompressMultipleFrame.h"
#include "ImageManager.h"

CompressMultipleFrame::CompressMultipleFrame(wxFrame* main_frame, const wxString& title) : 
    ImageOptionFrame(main_frame, title),
	scrolled_window(new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL)),
	scrolled_sizer(new wxBoxSizer(wxVERTICAL)), 
	button_panel(new wxPanel(this, wxID_ANY)),
    clusters_spinctrl(new wxSpinCtrl(button_panel, wxID_ANY,
        wxString::Format("%d", 8),
        wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS,
        1, 255, 8)),
    current_index_maps(nullptr),
	number_of_image_panels(0),
	image_display_panels(nullptr),
	resize_timer(new wxTimer(this, ID_ZoomTimer))
{   
    wxButton* open_and_compress_image_button = new wxButton(button_panel, wxID_ANY, "Open And Compress Multiple Image Files");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");
    wxStaticText* clusters_label = new wxStaticText(button_panel, wxID_ANY, "Number of colors:");

    open_and_compress_image_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::open_and_compress_multiple_images_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::return_to_menu_option, this);

    // bind resize event to resize the images in the panel
    this->Bind(wxEVT_SIZE, &CompressMultipleFrame::OnResize, this);
    this->Bind(wxEVT_TIMER, &CompressMultipleFrame::OnResizeTimer, this, ID_ZoomTimer);

    // for buttons
    wxBoxSizer* button_sizer = new wxWrapSizer(wxHORIZONTAL, wxWRAPSIZER_DEFAULT_FLAGS);
    button_sizer->Add(open_and_compress_image_button, 0, wxALL, 5);
    button_sizer->AddSpacer(10);
    button_sizer->Add(clusters_label, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    button_sizer->Add(clusters_spinctrl, 0, wxALL, 5);
    button_sizer->AddSpacer(10);
    button_sizer->Add(return_to_menu_button, 0, wxALL, 5);
    button_panel->SetSizer(button_sizer);

    // scrolled window
    scrolled_window->SetSizer(scrolled_sizer);

    // main sizer
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(button_panel, 0, wxEXPAND);
    main_sizer->Add(scrolled_window, 1, wxEXPAND);

    this->SetSizer(main_sizer);
    this->Layout();
    this->Show(true);
}

CompressMultipleFrame::~CompressMultipleFrame()
{
    if (resize_timer) delete resize_timer;
	delete[] image_display_panels;
	free_current_index_maps();
}

// called for when the user is resizing the frame
void CompressMultipleFrame::OnResize(wxSizeEvent& event)
{
    // if the user changes the state of the frame by clicking the restore window button, then resize the images immediately
    static bool wasMaximized = true;
    if (IsMaximized() != wasMaximized)
    {
        wasMaximized = IsMaximized();
        CallAfter([this]() 
            {
                for (size_t i = 0; i < number_of_image_panels; i++)
                    image_display_panels[i]->resize_images();
            }
        );
    }
    // else, continue the resize normally, but use a timer to delay the resizing until the user stops resizing the window 
    // after a short period
    else
    {
        resize_timer->Stop();
        resize_timer->StartOnce(30);
    }
    event.Skip();
}

void CompressMultipleFrame::OnResizeTimer(wxTimerEvent& /*event*/)
{
    for (size_t i = 0; i < number_of_image_panels; i++)
        image_display_panels[i]->resize_images();
}

void CompressMultipleFrame::open_and_compress_multiple_images_option(wxCommandEvent& /*event*/)
{
    wxArrayString new_image_paths = ImageManager::select_multiple_image_paths_option();
    
    // if the new image paths were valid
    if (!new_image_paths.empty())
    {
        // only delete the pointers to the image display panels; the destroy children method will delete them
		// clean up dynamic memory
        delete[] image_display_panels;
		free_current_index_maps();
        scrolled_window->DestroyChildren();

        // update everything to the new values
		number_of_image_panels = new_image_paths.GetCount();
        image_display_panels = new ImageDisplayPanel* [number_of_image_panels];
        current_index_maps = new const uint8_t* [number_of_image_panels];

		uint8_t number_of_clusters = clusters_spinctrl->GetValue();

        wxStaticLine* top_line = new wxStaticLine(scrolled_window, wxID_ANY);
		scrolled_sizer->Add(top_line, 0, wxEXPAND | wxALL, 5);

		// create a new display image panel with a unique save button assigned to it to save the compressed image as a .dat file
        // if pressed
        for (size_t i = 0; i < number_of_image_panels; i++)
        {
            // create the new image
			wxImage* new_initial_image = new wxImage(new_image_paths[i].ToStdString());
			ImageManager::remove_alpha_channels(new_initial_image); // only 3 color channels supported for now

			// get the metadata of the new image
			uint32_t image_width = new_initial_image->GetWidth();
			uint32_t image_height = new_initial_image->GetHeight();
			size_t number_of_pixels = image_width * image_height;
			std::filesystem::path new_image_path = new_image_paths[i].ToStdString();

			// cluster the data samples internally within the algorithm
            const uint8_t* new_initial_image_data = new_initial_image->GetData();
            wxBusyInfo* busy_info = new wxBusyInfo("Compressing " + new_image_path.filename().string() + "... " +
                "( " + std::to_string(i + 1) + " / " + std::to_string(number_of_image_panels) + " )");
            cluster_algorithm.cluster_data_samples(new_initial_image_data, number_of_pixels, number_of_clusters);
            delete busy_info;
			const uint8_t* algorithm_positions = cluster_algorithm.get_cluster_positions();
			const uint8_t* algorithm_index_map = cluster_algorithm.get_index_map();

            // generate the new compressed image
            wxImage* new_compressed_image = ImageManager::generate_compressed_image(Constants::NUMBER_OF_COLOR_CHANNELS, 
                number_of_clusters, algorithm_positions, image_width, image_height, algorithm_index_map);

            // create and display components to a new display panel
			bool use_high_quality_resizing = false;
            ImageDisplayPanel* new_display_panel = new ImageDisplayPanel(scrolled_window, new_image_path, image_width, image_height, 
                Constants::NUMBER_OF_COLOR_CHANNELS, number_of_clusters, algorithm_positions, use_high_quality_resizing);

            // set the images to the display panel
			new_display_panel->set_initial_image(new_initial_image);
            new_display_panel->set_compressed_image(new_compressed_image);

			// generate a button with a unique ID that will be used to save the compressed image as a .dat file
			wxFileName file_name = wxFileName(new_image_path.string());
			wxButton* save_compressed_image_button = new wxButton(scrolled_window, ID_NEWHIGHEST + static_cast<int>(i), 
                "Save Compressed " + file_name.GetFullName() + " Image As .dat File");
			save_compressed_image_button->Bind(wxEVT_BUTTON, &CompressMultipleFrame::save_compressed_image_option, this);

            // create a horizontal line
			wxStaticLine* bottom_line = new wxStaticLine(scrolled_window, wxID_ANY);

			// add everything to the sizer
			scrolled_sizer->Add(new_display_panel, 1, wxEXPAND | wxALL, 5);
			scrolled_sizer->Add(save_compressed_image_button, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
			scrolled_sizer->Add(bottom_line, 0, wxEXPAND | wxALL, 5);

            // assign the new panel and index map to the array of image display panels for saving later
            image_display_panels[i] = new_display_panel;

            uint8_t* temp_index_map = new uint8_t[number_of_pixels];
            std::memcpy(temp_index_map, algorithm_index_map, number_of_pixels * sizeof(uint8_t));
            current_index_maps[i] = temp_index_map;
            
			// clean up the dynamic memory here, as the display panel makes copies
            delete new_initial_image;
            delete new_compressed_image;
        }
        
        this->Layout();
        scrolled_window->Layout();
        scrolled_window->FitInside();

        // refresh each panel
        for (size_t i = 0; i < number_of_image_panels; i++)
            image_display_panels[i]->refresh_image_maps_and_descriptions();
    }
}

void CompressMultipleFrame::save_compressed_image_option(wxCommandEvent& event)
{
	// get the related objects that will be used to save the compressed image
	int file_index = event.GetId() - ID_NEWHIGHEST;
	const ImageDisplayPanel* current_image_display_panel = image_display_panels[file_index];
	const uint8_t* current_index_map = current_index_maps[file_index];
	const wxImage* current_compressed_image = current_image_display_panel->get_compressed_image();

    std::filesystem::path current_image_path = current_image_display_panel->get_current_image_path();
    wxFileName file_path(current_image_path.string());
    std::string image_file_name = file_path.GetName().ToStdString();

    uint8_t displayed_number_of_color_channels = current_image_display_panel->get_displayed_number_of_color_channels();
    uint8_t compressed_number_of_clusters = current_image_display_panel->get_compressed_number_of_clusters();
    const uint8_t* compressed_cluster_positions = current_image_display_panel->get_compressed_cluster_positions();
    uint32_t displayed_width = current_compressed_image->GetWidth();
    uint32_t displayed_height = current_compressed_image->GetHeight();

    if (!ImageManager::save_image_as_dat(image_file_name, displayed_number_of_color_channels, compressed_number_of_clusters,
        compressed_cluster_positions, displayed_width, displayed_height, current_index_map))
    {
        wxMessageBox("Failed to save the compressed image!", "Save Warning", wxOK | wxICON_WARNING);
        return;
    }

    wxMessageBox("Saved to " + (ImageManager::default_dat_output_path / (image_file_name + ".dat")).string(), "Saved", wxOK | wxICON_INFORMATION);
}

void CompressMultipleFrame::free_current_index_maps()
{
    if (current_index_maps)
    {
        for (size_t i = 0; i < number_of_image_panels; i++)
            if (current_index_maps[i]) delete[] current_index_maps[i];
        delete[] current_index_maps;
        current_index_maps = nullptr;
    }
}