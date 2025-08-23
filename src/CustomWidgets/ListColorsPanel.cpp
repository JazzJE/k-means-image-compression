#include <string.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>

#include "CustomWidgets/ListColorsPanel.h"

ListColorsPanel::ListColorsPanel(wxWindow* parent, uint8_t number_of_color_channels, uint8_t number_of_colors, 
	const uint8_t* color_values) :
	
	wxPanel(parent),
	number_of_colors(number_of_colors),
	number_of_color_channels(number_of_color_channels)

{
	wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
	
	// title
	main_sizer->Add(new wxStaticText(this, wxID_ANY, "Colors List:"), 0, wxALIGN_LEFT | wxALL, 5);

	// displayed colors
	scrolled_sizer = new wxBoxSizer(wxHORIZONTAL);
	scrolled_window = new wxScrolledWindow(this);
	scrolled_window->SetVirtualSize(1000, -1);
	scrolled_window->SetScrollRate(5, 0);
	scrolled_window->SetMinSize(wxSize(200, -1));
	scrolled_window->SetSizer(scrolled_sizer);
	main_sizer->Add(scrolled_window, 1, wxEXPAND | wxALL, 10);

	this->SetSizer(main_sizer);

	update_colors(number_of_color_channels, number_of_colors, color_values);
}

ListColorsPanel::~ListColorsPanel()
{ scrolled_sizer->Clear(true); }

// used to change the colors displayed in the sizer and the associated labels
// number_of_colors by default is 0, so if it is not specified, then the panel will not display any colors
void ListColorsPanel::update_colors(uint8_t new_number_of_color_channels, uint8_t new_number_of_colors, const uint8_t* new_color_values)
{
	scrolled_sizer->Clear(true);

	// update the number of colors and color channels to the new values
	number_of_colors = new_number_of_colors;
	number_of_color_channels = new_number_of_color_channels;

	// if there are no colors, no data, or no color channels, then just return with nothing
	if (number_of_colors == 0 || new_color_values == nullptr || number_of_color_channels == 0)
	{
		scrolled_window->FitInside();
		this->Show(false);
		this->Layout();
		return;
	}

	// for each color, create a panel that will have a static bitmap and a label, then add that panel to this panel
	// the bitmap will be a 20x20 pixel image of the color and the label will be the RGB values
	for (size_t n = 0; n < number_of_colors; ++n)
	{
		uint8_t* new_color_data = (uint8_t*)malloc(number_of_color_channels * sizeof(uint8_t));
		size_t color_first_feature_index = n * number_of_color_channels;

		// if any of the colors data failed to load, then delete everything up to this point and return
		if (!new_color_data)
		{
			scrolled_sizer->Clear(true);
			wxMessageBox("Failed to allocate memory for color data.", "Error", wxOK | wxICON_ERROR, this);
			return;
		}

		// image
		for (size_t c = 0; c < number_of_color_channels; ++c)
			new_color_data[c] = new_color_values[color_first_feature_index + c];
		wxImage new_image(1, 1, new_color_data, false);
		new_image.Rescale(20, 20, wxIMAGE_QUALITY_NORMAL);
		wxStaticBitmap* new_bitmap = new wxStaticBitmap(scrolled_window, wxID_ANY, wxBitmap(new_image));

		// label
		std::string new_color_label_text = "(";
		for (size_t c = 0; c < number_of_color_channels; ++c)
		{
			new_color_label_text += std::to_string(new_color_values[color_first_feature_index + c]);
			if (c < number_of_color_channels - 1)
				new_color_label_text += ", ";
		}
		new_color_label_text += ")";
		wxStaticText* new_text = new wxStaticText(scrolled_window, wxID_ANY, new_color_label_text);

		scrolled_sizer->Add(new_bitmap, 0,  wxALL, 5);
		scrolled_sizer->Add(new_text, 0, wxALL, 5);
	}

	scrolled_window->FitInside();
	this->Show(true);
	this->Layout();
}