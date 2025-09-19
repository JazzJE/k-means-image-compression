#pragma once
#include <wx/frame.h>
#include <wx/image.h>
#include <wx/spinctrl.h>
#include <wx/event.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/scrolwin.h>
#include <wx/statbmp.h>
#include <wx/timer.h>
#include <filesystem>

#include "CustomWidgets/ImageDisplayPanel.h"
#include "Frames/OptionFrames/ImageOptionFrame.h"
#include "KClusterAlgorithm.h"

class CompressSingleFrame : public ImageOptionFrame
{
private:

	KClusterAlgorithm cluster_algorithm;

	wxPanel* const button_panel;
	wxSpinCtrl* const clusters_spinctrl;

	ImageDisplayPanel* const image_display_panel;
	const uint8_t* current_index_map; // this points to the index map inside of the clustering algorithm; thus, it should not be deleted here

	enum { ID_ZoomTimer = wxID_HIGHEST + 1 };
	wxTimer* const resize_timer;

	void open_image_file_option(wxCommandEvent&);
	void save_transformed_image_as_dat_option(wxCommandEvent&);
	void save_transformed_image_as_png_option(wxCommandEvent&);
	void generate_compressed_image_option(wxCommandEvent&);

	// for updating image sizes when the frame is resized or changes dimensions
	void OnResize(wxSizeEvent&);
	void OnResizeTimer(wxTimerEvent&);

public:

	CompressSingleFrame(wxFrame* main_frame, const wxString& title);
	~CompressSingleFrame();

};