#pragma once
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/wrapsizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <cstdint>

class ListColorsPanel : public wxPanel
{
private:

	uint8_t number_of_colors;
	uint8_t number_of_color_channels;

	// used for displaying the colors in the panel
	wxScrolledWindow* scrolled_window; 
	wxBoxSizer* scrolled_sizer;

public:

	ListColorsPanel(wxWindow* parent, uint8_t number_of_color_channels = 0, uint8_t number_of_colors = 0, 
		const uint8_t* color_values = nullptr);
	~ListColorsPanel();

	void update_colors(uint8_t new_number_of_color_channels, uint8_t new_number_of_colors, const uint8_t* new_color_values);

};