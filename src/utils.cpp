#include "utils.h"

wxString select_image_file_option()
{
    wxFileDialog* open_dialog = new wxFileDialog(nullptr,
        "Open Image File",
        "",
        "",
        "Image files (*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tga)|*.png;*.jpg;*.jpeg;*.bmp;*.gif;*.tiff;*.tga|"
        "PNG files (*.png)|*.png|"
        "JPEG files (*.jpg;*.jpeg)|*.jpg;*.jpeg|"
        "BMP files (*.bmp)|*.bmp|"
        "GIF files (*.gif)|*.gif|"
        "TIFF files (*.tiff)|*.tiff|"
        "TGA files (*.tga)|*.tga",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (open_dialog->ShowModal() == wxID_CANCEL) {
        delete open_dialog;
        return "";
    }

    wxString path = open_dialog->GetPath();
    wxMessageBox("Selected file: " + path, "File Selected", wxOK | wxICON_INFORMATION);
    delete open_dialog;

    return path;
}

wxString select_dat_file_option()
{
    wxFileDialog* open_dialog = new wxFileDialog(nullptr,
        "Open .dat Image File",
        "",
        "",
        "DAT files (*.dat)|*.dat",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (open_dialog->ShowModal() == wxID_CANCEL) {
        delete open_dialog;
        return "";
    }
    wxString path = open_dialog->GetPath();
    wxMessageBox("Selected file: " + path, "File Selected", wxOK | wxICON_INFORMATION);
    delete open_dialog;

    return path;
}