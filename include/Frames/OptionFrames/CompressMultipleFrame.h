#pragma once
#include <string>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/scrolwin.h>
#include <wx/gbsizer.h>
#include <wx/event.h>
#include <wx/timer.h>
#include <wx/spinctrl.h>

#include "Frames/OptionFrames/ImageOptionFrame.h"
#include "CustomWidgets/ImageDisplayPanel.h"
#include "KClusterAlgorithm.h"

class CompressMultipleFrame : public ImageOptionFrame
{
private:

	KClusterAlgorithm cluster_algorithm;
	
	wxPanel* const button_panel;
	wxSpinCtrl* const clusters_spinctrl;

	wxScrolledWindow* const scrolled_window;
	wxBoxSizer* const scrolled_sizer;

	size_t number_of_image_panels;
	const uint8_t** current_index_maps;

	enum { ID_ZoomTimer = wxID_HIGHEST + 1, ID_NEWHIGHEST = ID_ZoomTimer + 1 };
	wxTimer* const resize_timer;

	void open_multiple_images_option(wxCommandEvent&);
	void generate_compressed_images_option(wxCommandEvent& event);
	void save_compressed_image_as_dat_option(wxCommandEvent& event);
	void save_compressed_image_as_png_option(wxCommandEvent& event);

	// used for resizing all image panels when the frame is resized or changes dimensions
	ImageDisplayPanel** image_display_panels;
	void OnResize(wxSizeEvent&);
	void OnResizeTimer(wxTimerEvent&);

	void free_current_index_maps();
	void resize_panels();

public:

	CompressMultipleFrame(wxFrame* main_frame, const wxString& title);
	~CompressMultipleFrame();

};