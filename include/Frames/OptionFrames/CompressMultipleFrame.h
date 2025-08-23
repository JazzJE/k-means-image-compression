#pragma once
#include <string>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/scrolwin.h>
#include <wx/gbsizer.h>
#include <wx/event.h>

#include "Frames/OptionFrames/ImageOptionFrame.h"
#include "CustomWidgets/ImageDisplayPanel.h"

class CompressMultipleFrame : public ImageOptionFrame
{
private:

	wxScrolledWindow* const scrolled_window;
	wxBoxSizer* const scrolled_sizer;

	wxArrayString image_paths;
	ImageDisplayPanel** image_display_panels;
	const uint8_t** current_index_maps;

	void open_image_file_option(wxCommandEvent&);
	void generate_compressed_images_option(wxCommandEvent&);

	// used for resizing all image panels when the frame is resized or changes dimensions
	void OnResize(wxSizeEvent& event);
	void OnMinimize(wxIconizeEvent& event);

public:

	CompressMultipleFrame(wxFrame* main_frame, const wxString& title);

};