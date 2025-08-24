#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/font.h>
#include <wx/event.h>
#include <wx/string.h>

#include "Frames/MainFrame.h"
#include "Frames/OptionFrames/CompressMultipleFrame.h"
#include "Frames/OptionFrames/CompressSingleFrame.h"
#include "Frames/OptionFrames/DisplayImageFrame.h"
#include "Constants.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
    wxPanel* title_panel = new wxPanel(this);
    wxStaticText* title_text = new wxStaticText(title_panel, wxID_ANY, "K-Means Clustering Compression Program");
    wxStaticText* subtitle_text = new wxStaticText(title_panel, wxID_ANY, "A Project Made By JazzJE");

    // Make title text larger and bold
    wxFont titleFont = title_text->GetFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 4);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    title_text->SetFont(titleFont);

    // Make subtitle smaller and italic
    wxFont subtitleFont = subtitle_text->GetFont();
    subtitleFont.SetPointSize(subtitleFont.GetPointSize() - 1);
    subtitleFont.SetStyle(wxFONTSTYLE_ITALIC);
    subtitle_text->SetFont(subtitleFont);

    wxPanel* button_panel = new wxPanel(this);

    // Create buttons with better sizing
    wxButton* single_images_button = new wxButton(button_panel, wxID_ANY, "Compress Singular Image",
        wxDefaultPosition, Constants::DEFAULT_BUTTON_SIZE);
    wxButton* multiple_images_button = new wxButton(button_panel, wxID_ANY, "Compress Multiple Images",
        wxDefaultPosition, Constants::DEFAULT_BUTTON_SIZE);
    wxButton* display_image_button = new wxButton(button_panel, wxID_ANY, "Display .dat Image File",
        wxDefaultPosition, Constants::DEFAULT_BUTTON_SIZE);

    // Bind events
    single_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_single_image, this);
    multiple_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_multiple_images, this);
    display_image_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::display_image, this);

    // Title panel layout with both title and subtitle
    wxBoxSizer* title_sizer = new wxBoxSizer(wxVERTICAL);
    title_sizer->AddSpacer(20);
    title_sizer->Add(title_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);
    title_sizer->Add(subtitle_text, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 5);
    title_sizer->AddSpacer(15);
    title_panel->SetSizer(title_sizer);

    // Button panel layout
    wxBoxSizer* button_sizer = new wxBoxSizer(wxVERTICAL);
    button_sizer->AddSpacer(20);
    button_sizer->Add(single_images_button, 0, wxALIGN_CENTER | wxALL, 8);
    button_sizer->Add(multiple_images_button, 0, wxALIGN_CENTER | wxALL, 8);
    button_sizer->Add(display_image_button, 0, wxALIGN_CENTER | wxALL, 8);
    button_sizer->AddStretchSpacer(1);
    button_panel->SetSizer(button_sizer);

    // Main frame layout
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(title_panel, 0, wxEXPAND);
    main_sizer->Add(button_panel, 1, wxEXPAND);
    this->SetSizer(main_sizer);

    // Set minimum frame size (keeps your minimum size requirement)
    this->SetMinSize(Constants::MINIMUM_WINDOW_SIZE);

    this->Center();
}

void MainFrame::compress_single_image(wxCommandEvent& /*event*/)
{
    this->Show(false);
    wxString title = "Compress Single Image";
    current_frame = new CompressSingleFrame(this, title);
    default_configs_for_current_frame();
}

void MainFrame::compress_multiple_images(wxCommandEvent& /*event*/)
{
    this->Show(false);
    wxString title = "Compress Multiple Images";
    current_frame = new CompressMultipleFrame(this, title);
    default_configs_for_current_frame();
}

void MainFrame::display_image(wxCommandEvent& /*event*/)
{
    this->Show(false);
    wxString title = "Display .dat Image";
    current_frame = new DisplayImageFrame(this, title);
    default_configs_for_current_frame();
}

// edit the current frame to have these specific display options
void MainFrame::default_configs_for_current_frame()
{
    current_frame->SetMinSize(Constants::MINIMUM_WINDOW_SIZE);
    current_frame->Maximize();
    current_frame->Center();
    current_frame->Raise();
    current_frame->SetFocus();
    current_frame->Show();
}