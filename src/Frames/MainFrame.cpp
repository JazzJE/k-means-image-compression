#include "Frames/MainFrame.h"
#include "Frames/CompressMultipleFrame.h"
#include "Frames/CompressSingleFrame.h"
#include "Constants.h"
#include <wx/wx.h>
#include <wx/spinctrl.h>

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

    wxPanel* title_panel = new wxPanel(this);

    wxStaticText* title_text = new wxStaticText(title_panel, wxID_ANY, "K-Means Clustering Compression Program");

    wxPanel* button_panel = new wxPanel(this);

    // create the two button options
    wxButton* single_images_button = new wxButton(button_panel, wxID_ANY, "Compress Singular Image");
    wxButton* multiple_images_button = new wxButton(button_panel, wxID_ANY, "Compress Multiple Images");

    single_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_single_image, this);
    multiple_images_button->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::compress_multiple_images, this);

    // 

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
    this->Show(false);

    wxString title = "Compress Single Image";
    compress_frame = new CompressSingleFrame(title);
    compress_frame->SetClientSize(Constants::screen_width, Constants::screen_height);
    compress_frame->Center();
    compress_frame->Show();
    compress_frame->Raise();
    compress_frame->SetFocus();

    // Bind to the close event of the compress frame
    compress_frame->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCompressFrameClose, this);
}

void MainFrame::compress_multiple_images(wxCommandEvent& /*event*/)
{
    this->Show(false);

    wxString title = "Compress Multiple Images";
    CompressMultipleFrame* compress_frame = new CompressMultipleFrame(title);
    compress_frame->SetClientSize(Constants::screen_width, Constants::screen_height);
    compress_frame->Center();
    compress_frame->Show();
    compress_frame->Raise();
    compress_frame->SetFocus();

    // bind the closing of the compression frame such that the main option frame reopens upon close
    compress_frame->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnCompressFrameClose, this);
}

void MainFrame::OnCompressFrameClose(wxCloseEvent& event)
{
    compress_frame = nullptr;
    event.Skip();

    this->Show();
    this->Raise();
    this->SetFocus();
}