#pragma once
#include <cstdint>
#include <wx/gdicmn.h>

namespace Constants 
{
	constexpr uint8_t number_of_color_channels = 3;

	const wxSize minimum_window_size(600, 425);
	const wxSize default_button_size(300, 70);

	constexpr uint32_t DAT_FILE_MAGIC_NUMBER = 0xDEADBEEF;
}