#pragma once
#include <wx/wx.h>
#include <wx/filedlg.h>
#include <wx/filename.h>

// used for returning a wximage once a file is selected; this includes verifying the image path is valid, else the value returned is nullptr
wxImage* load_user_selected_file(wxString (*select_file_option)());

// used for getting a file path from the user which goes to a file
wxString select_image_path_option();
wxString select_dat_path_option();