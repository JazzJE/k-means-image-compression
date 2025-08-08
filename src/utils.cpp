#include "utils.h"
#include "ImageManager.h"

wxImage* load_user_selected_file(wxString(*select_file_option)())
{
    wxString file_path = select_file_option();

    // verify the file path is valid; return nullptr if invalid
    if (file_path.empty()) 
    {
        wxMessageBox("File path is empty.");
        return nullptr;
    }

    wxFileName file(file_path);

    if (!file.FileExists()) 
    {
        wxMessageBox("File does not exist: %s", file_path);
        return nullptr;
    }

    if (!file.IsFileReadable()) 
    {
        wxMessageBox("File is not readable or is a directory: %s", file_path);
        return nullptr;
    }

    wxImage* created_image = new wxImage();
    created_image->LoadFile(file_path);

    return created_image;
}

wxString select_image_path_option()
{
    wxFileDialog open_dialog(nullptr,
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

    if (open_dialog.ShowModal() == wxID_CANCEL)
        return "";

    return open_dialog.GetPath();
}

wxString select_dat_path_option()
{
    wxString restricted_path = ImageManager::get_default_image_output_path().c_str();

    wxFileDialog open_dialog(nullptr,
        "Open .dat Image File",
        restricted_path,
        "",
        "DAT files (*.dat)|*.dat",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (open_dialog.ShowModal() == wxID_CANCEL)
        return "";

    return open_dialog.GetPath();
}