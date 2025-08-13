#include "utils.h"
#include "ImageManager.h"

void reset_frame(wxFrame* frame)
{
    // Clear sizer first if one exists
    if (wxSizer* sizer = frame->GetSizer()) 
    {
        sizer->Clear(true); // true = delete windows
        frame->SetSizer(nullptr); // Remove the sizer
    }

    // Destroy all children
    frame->DestroyChildren();

    // Force refresh
    frame->Layout();
    frame->Refresh();
}

// remove the alpha channels if the image has one
void remove_alpha_channels(wxImage* image)
{ if (image->HasAlpha()) image->ClearAlpha(); }