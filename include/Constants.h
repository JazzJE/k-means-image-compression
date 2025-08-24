#pragma once
#include <cstdint>
#include <wx/gdicmn.h>

namespace Constants 
{
	constexpr uint8_t NUMBER_OF_COLOR_CHANNELS = 3;

	const wxSize MINIMUM_WINDOW_SIZE(600, 425);
	const wxSize DEFAULT_BUTTON_SIZE(300, 70);

	constexpr uint32_t DAT_FILE_MAGIC_NUMBER = 0xDEADBEEF;

	constexpr double IMAGE_ZOOM_STEP = 0.1;
	constexpr double IMAGE_MIN_ZOOM_FACTOR = 0.5;
	constexpr double IMAGE_MAX_ZOOM_FACTOR = 2.0;
}