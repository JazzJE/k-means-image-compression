#include <omp.h>

#include "Frames/CompressSingleFrame.h"
#include "utils.h"

CompressSingleFrame::CompressSingleFrame(const wxString& title, wxFrame* main_frame) : ImageOptionFrame(title, main_frame), 
displayed_height(0), displayed_number_of_color_channels(Constants::number_of_color_channels), displayed_width(0), initial_image(nullptr),
compressed_cluster_positions(nullptr), compressed_image(nullptr), compressed_number_of_clusters(0), current_image_path(""),
compressed_index_map(nullptr), clusters_spinctrl(nullptr)
{ draw_frame(); }

void CompressSingleFrame::draw_frame()
{
    reset_frame(this);

    wxPanel* button_panel = new wxPanel(this);
    wxScrolledWindow* image_scroller = new wxScrolledWindow(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL | wxHSCROLL);
    image_scroller->SetScrollRate(10, 10); // scroll step in pixels

    wxStaticBitmap* initial_image_map;
    wxStaticBitmap* transformed_image_map;

    // load the initial image
    wxBitmap current_bitmap = wxNullBitmap;
    if (initial_image)
        current_bitmap = wxBitmap(*initial_image);
    initial_image_map = new wxStaticBitmap(image_scroller, wxID_ANY, current_bitmap);

    // load the transformed image
    current_bitmap = wxNullBitmap;
    if (compressed_image)
        current_bitmap = wxBitmap(*compressed_image);
    transformed_image_map = new wxStaticBitmap(image_scroller, wxID_ANY, current_bitmap);

    wxButton* open_image_file_button = new wxButton(button_panel, wxID_ANY, "Open Image File");
    wxButton* save_transformed_image_button = new wxButton(button_panel, wxID_ANY, "Save Transformed Image");
    wxButton* generate_compressed_image_button = new wxButton(button_panel, wxID_ANY, "Generate Compressed Image");
    wxButton* return_to_menu_button = new wxButton(button_panel, wxID_ANY, "Return to Menu");

    wxStaticText* clusters_label = new wxStaticText(button_panel, wxID_ANY, "Number of colors:");
    clusters_spinctrl = new wxSpinCtrl(button_panel, wxID_ANY, "8",
        wxDefaultPosition, wxSize(80, -1), wxSP_ARROW_KEYS, 1, 255, 8);

    open_image_file_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::open_image_file_option, this);
    save_transformed_image_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::save_transformed_image_option, this);
    generate_compressed_image_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::generate_compressed_image_option, this);
    return_to_menu_button->Bind(wxEVT_BUTTON, &CompressSingleFrame::return_to_menu_option, this);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* image_sizer = new wxBoxSizer(wxHORIZONTAL);

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

    // images
    image_sizer->Add(initial_image_map, 0, wxALL, 5);
    image_sizer->Add(transformed_image_map, 0, wxALL, 5);
    image_scroller->SetSizer(image_sizer);
    image_scroller->FitInside(); // makes sure scrollbars know the content size

    // layout
    main_sizer->Add(button_panel, 0, wxEXPAND);   // buttons above
    main_sizer->Add(image_scroller, 1, wxEXPAND); // images below

    this->SetSizer(main_sizer);
    this->Layout();
    this->Show(true);
}

void CompressSingleFrame::open_image_file_option(wxCommandEvent& /*event*/)
{
    current_image_path = ImageManager::select_image_path_option().ToStdString();

    // delete the current image objects if they exist before loading new images
    if (initial_image)
    { delete initial_image; initial_image = nullptr; }
    if (compressed_image)
    { delete compressed_image; compressed_image = nullptr; }

    if (!current_image_path.empty())
        initial_image = new wxImage(current_image_path.string());

    draw_frame();
}

void CompressSingleFrame::save_transformed_image_option(wxCommandEvent& /*event*/)
{
    if (compressed_image)
    {
        wxFileName file_path(current_image_path.string());
        std::string file_name = file_path.GetName().ToStdString();

        ImageManager::save_image_as_dat(file_name, displayed_number_of_color_channels, compressed_number_of_clusters, 
            compressed_cluster_positions, displayed_width, displayed_height, compressed_index_map);
    }

    else
        wxMessageBox("There is no transformed image to save!", "Save Warning", wxOK | wxICON_WARNING);
}

void CompressSingleFrame::generate_compressed_image_option(wxCommandEvent& /*event*/)
{
    if (initial_image != nullptr)
    {
        compressed_number_of_clusters = clusters_spinctrl->GetValue();
        size_t number_of_pixels = displayed_width * displayed_height;
        uint8_t* image_data = initial_image->GetData();

        // cluster the data samples internally within the algorithm
        cluster_algorithm.cluster_data_samples(image_data, number_of_pixels, compressed_number_of_clusters);

        // then get the resulting index map and cluster positions to construct the data of the transformed image
        uint8_t* generated_cluster_positions = cluster_algorithm.get_cluster_positions();
        uint8_t* generated_index_map = cluster_algorithm.get_index_map();

        #pragma omp parallel for
        for (int p = 0; p < number_of_pixels; p++)
        {
            size_t pixel_first_feature_index = p * displayed_number_of_color_channels;
            size_t cluster_first_feature_index = generated_index_map[p] * displayed_number_of_color_channels;
            for (size_t f = 0; f < displayed_number_of_color_channels; f++)
                image_data[pixel_first_feature_index + f] = generated_cluster_positions[cluster_first_feature_index + f];
        }

        compressed_image;
    }
    else
        wxMessageBox("There is no initial image to transform!", "Generate Warning", wxOK | wxICON_WARNING);
}