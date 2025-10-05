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
- Image loading and saving with custom .dat formats that drastically reduce file size
- Simple GUI design with wxWidgets for real-time compression, interaction and visualization

# Setup

## Requirements

- **CMake ≥ 3.16**
- A **C++17 compiler**  
- (Optional) Installed system versions of:
  - **OpenMP**  
  - **wxWidgets v3.2.x** (detected automatically if installed via vcpkg or system package manager)

__If not found, both OpenMP and wxWidgets will be fetched and built from source.__

## Setup Guide

1. **Clone the repository:**
   ```bash
   git clone https://github.com/JazzJE/k-means-image-compression
	```

2. **Build the project:**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. **Run the program**
   ```bash
   ./k_means_image_compression
   ```

# Usage

1. Use the GUI to load an image, set the number of clusters, and compress an image  

<br>

   ![Compress Image](https://github.com/JazzJE/k-means-image-compression/blob/main/assets/single%20image%20example.png?raw=true)

<br><br>

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