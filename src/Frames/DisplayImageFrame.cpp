#include "Frames/DisplayImageFrame.h"
#include "utils.h"

DisplayImageFrame::DisplayImageFrame(const wxString& title) : ImageOptionFrame(title)
{
    select_image_file_option();
}