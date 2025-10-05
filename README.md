# K-Clustering Image Compression

An implementation of K-means clustering for image compression using C++ and no external ML libraries. This project, again, is a pure demonstration of my appreciation for machine learning and is not intended for production use. I still love raw pointers, but I'll start implementing smart ones in my next projects.

## Table of Contents

- [Features](#features)
- [Setup](#setup)
- [Usage](#usage)
- [Example](#example)
- [License](#license)

# Features

- K-means++ initialization for better clustering
- Customizable number of clusters using a spin control
- Image loading and saving with custom .dat formats that displays color metadata
- Simple GUI design with wxWidgets for real-time compression, interaction and visualization

# Setup

## Requirements

- **CMake ≥ 3.16**  
- A **C++17 compiler**  
- **OpenMP** (optional; will use system version if available)  
- **wxWidgets v3.2.x** (must be installed on your system)

> **Important:** Users must install wxWidgets manually. On different platforms:  
> - **Linux (Debian/Ubuntu/Kali):** `sudo apt install libwxgtk3.0-gtk3-dev libgtk-3-dev`  
> - **macOS:** `brew install wxwidgets`  
> - **Windows:** Use [vcpkg](https://github.com/microsoft/vcpkg) or a pre-built installer.

## Setup Guide

1. **Clone the repository:**
   ```bash
   git clone https://github.com/JazzJE/k-means-image-compression
	```

2. **Install dependencies**

   - Linux (Debian/Ubuntu/Kali)

   ```bash
   sudo apt update && sudo apt install -y build-essential cmake libwxgtk3.0-gtk3-dev libgtk-3-dev
   ```

   - macOS

   ```bash
   brew update && brew install cmake wxwidgets
   ```

   > **Note:** You may need to add wxWidgets to your PATH on macOS:
   ```bash
   export PATH="/opt/homebrew/opt/wxwidgets/bin:$PATH"
   ```

   - Windows

   ```powershell
   git clone https://github.com/microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg install wxwidgets:x64-windows
   ```

   Then configure CMake to use vcpkg:

   ```powershell
   cmake .. -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```
   

3. **Build the project:**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

4. **Run the program**
   ```bash
   ./k_means_image_compression
   ```

# Usage

1. Use the GUI to load an image, set the number of clusters, and compress an image  

<br>

   ![Compress Image](https://github.com/JazzJE/k-means-image-compression/blob/main/assets/single%20image%20example.png?raw=true)

<br>

2. Save any compressed images that you want to keep in a `.PNG` or custom `.DAT` format  
   (the `.DAT` file can only be opened using the display option of the program)  
  
<br>

   ![Display Image](https://github.com/JazzJE/k-means-image-compression/blob/main/assets/display%20example.png?raw=true)

# Example

<p align="center">
  <img src="https://github.com/JazzJE/k-means-image-compression/blob/main/assets/original%20image.jpg?raw=true"
       width="45%" alt="Original Image (3.78 MB)">
  <img src="https://github.com/JazzJE/k-means-image-compression/blob/main/assets/transformed%20image.png?raw=true"
       width="45%" alt="Compressed Image (2.55 MB)">
</p>

<p align="center">
  <strong>Original - 3.78 MB</strong> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
  <strong>Compressed (K-Means) - 2.55 MB</strong>
</p>

<p align="center">
  <em>Compression reduced file size by approximately 33% with minimal quality loss.</em>
</p>