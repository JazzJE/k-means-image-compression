#include "Frames/MainFrame.h"
#include "Frames/CompressMultipleFrame.h"
#include "Frames/CompressSingleFrame.h"
#include "Constants.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
    /*

    wxPanel* title_panel = new wxPanel(this);

    wxStaticText* title_text = new wxStaticText(title_panel, wxID_ANY, "K-Means Clustering Compression Program");
    */
    wxPanel* button_panel = new wxPanel(this);

    // create the three button options
    wxButton* single_images_button = new wxButton(button_panel, wxID_ANY, "Compress Singular Image", wxPoint(100, 50));
    wxButton* multiple_images_button = new wxButton(button_panel, wxID_ANY, "Compress Multiple Images", wxPoint(100, 100));
    wxButton* display_image_button = new wxButton(button_panel, wxID_ANY, "Display .dat Image File", wxPoint(300, 50));

    single_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_single_image, this);
    multiple_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_multiple_images, this);
    display_image_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::display_image, this);

	/*
	wxButton* button = new wxButton(panel, wxID_ANY, "Button", wxPoint(150, 50), wxSize(100, 35));

	wxStaticText* text = new wxStaticText(panel, wxID_ANY, "StaticText - NOT editable", wxPoint(120, 150));

	wxGauge* loading_bar = new wxGauge(panel, wxID_ANY, 100, wxPoint(500, 255), wxSize(200, -1));
	loading_bar->SetValue(50);

	wxSpinCtrl spin_ctrl = new wxSpinCtrl(panel, wxID_ANY, "", wxPoint(550, 375), wxSize(100, -1));
	*/
}

void MainFrame::compress_single_image(wxCommandEvent& /*event*/)
{
    wxString title = "Compress Single Image";
    current_frame = new CompressSingleFrame(title);
    config_current_frame();
}

void MainFrame::compress_multiple_images(wxCommandEvent& /*event*/)
{
    wxString title = "Compress Multiple Images";
    current_frame = new CompressMultipleFrame(title);
    config_current_frame();
}

void MainFrame::display_image(wxCommandEvent& /*event*/)
{
    wxString title = "Display .dat Image";
    current_frame = new CompressMultipleFrame(title);
    config_current_frame();
}

// edit the current frame to have these specific display options
void MainFrame::config_current_frame()
{
    this->Show(false);
    current_frame->SetClientSize(Constants::screen_width, Constants::screen_height);
    current_frame->Center();
    current_frame->Show();
    current_frame->Raise();
    current_frame->SetFocus();

    // bind the closing of the compression frame such that the main option frame reopens upon close
    current_frame->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCompressFrameClose, this);
}

void MainFrame::OnCompressFrameClose(wxCloseEvent& event)
{
    current_frame = nullptr;
    event.Skip();

    this->Show();
    this->Raise();
    this->SetFocus();
}