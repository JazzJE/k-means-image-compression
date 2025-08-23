cmake_minimum_required(VERSION 3.16)
project(k_means_cluster_image_compression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and setup OpenMP library (REQUIRED)
message(STATUS "Setting up OpenMP...")

find_package(OpenMP QUIET)

if(NOT OpenMP_FOUND OR NOT OpenMP_CXX_FOUND)
    message(STATUS "OpenMP not found, attempting to download and build LLVM OpenMP...")
    
    # Download LLVM OpenMP runtime
    FetchContent_Declare(
        openmp
        GIT_REPOSITORY https://github.com/llvm/llvm-project.git
        GIT_TAG llvmorg-17.0.6  # Use stable version
        GIT_SHALLOW TRUE
        SOURCE_SUBDIR openmp
    )
    
    # OpenMP build options
    set(OPENMP_ENABLE_LIBOMPTARGET OFF CACHE BOOL "Enable OpenMP target offloading")
    set(OPENMP_ENABLE_LIBOMP_PROFILING OFF CACHE BOOL "Enable OpenMP profiling")
    set(OPENMP_STANDALONE_BUILD ON CACHE BOOL "OpenMP standalone build")
    
    # Disable tests and examples for faster build
    set(OPENMP_ENABLE_TESTING OFF CACHE BOOL "Enable OpenMP testing")
    set(OPENMP_ENABLE_WERROR OFF CACHE BOOL "Enable OpenMP Werror")
    
    FetchContent_MakeAvailable(openmp)
    
    # Create target alias for consistency
    if(TARGET omp)
        add_library(OpenMP::OpenMP_CXX ALIAS omp)
        set(OpenMP_CXX_FOUND TRUE)
        message(STATUS "OpenMP downloaded and built from LLVM source")
        message(STATUS "OpenMP location: ${openmp_SOURCE_DIR}")
    else()
        message(FATAL_ERROR "Failed to build OpenMP from source. Please install OpenMP manually.")
    endif()
else()
    message(STATUS "OpenMP found via system installation")
    message(STATUS "OpenMP version: ${OpenMP_CXX_VERSION}")
endif()

# Download and setup wxWidgets library (REQUIRED)
message(STATUS "Setting up wxWidgets...")

# Try to find wxWidgets first (in case it's already installed via vcpkg)
find_package(wxWidgets CONFIG QUIET)

if(NOT wxWidgets_FOUND)
    message(STATUS "wxWidgets not found via vcpkg, downloading from source...")
    
    FetchContent_Declare(
        wxwidgets
        GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
        GIT_TAG v3.2.4  # Use stable version
        GIT_SHALLOW TRUE
    )
    
    # wxWidgets build options
    set(wxBUILD_SHARED OFF CACHE BOOL "Build wxWidgets as shared library")
    set(wxBUILD_SAMPLES OFF CACHE BOOL "Build wxWidgets samples")
    set(wxBUILD_TESTS OFF CACHE BOOL "Build wxWidgets tests")
    set(wxBUILD_DEMOS OFF CACHE BOOL "Build wxWidgets demos")
    set(wxBUILD_BENCHMARKS OFF CACHE BOOL "Build wxWidgets benchmarks")
    
    # Only build what we need
    set(wxUSE_GUI ON CACHE BOOL "Use GUI")
    set(wxUSE_WEBVIEW OFF CACHE BOOL "Use WebView")
    set(wxUSE_MEDIACTRL OFF CACHE BOOL "Use Media Control")
    set(wxUSE_RIBBON OFF CACHE BOOL "Use Ribbon")
    set(wxUSE_PROPGRID OFF CACHE BOOL "Use Property Grid")
    set(wxUSE_RICHTEXT OFF CACHE BOOL "Use Rich Text")
    set(wxUSE_STC OFF CACHE BOOL "Use Styled Text Control")
    set(wxUSE_XRC OFF CACHE BOOL "Use XRC")
    
    FetchContent_MakeAvailable(wxwidgets)
    
    # Create aliases to match vcpkg naming
    add_library(wx::core ALIAS wxcore)
    add_library(wx::base ALIAS wxbase)
    
    message(STATUS "wxWidgets downloaded and built from source")
    message(STATUS "wxWidgets location: ${wxwidgets_SOURCE_DIR}")
else()
    message(STATUS "wxWidgets found via vcpkg")
endif()

# Source files
set(SOURCES
    src/KClusterAlgorithm.cpp
    src/Frames/MainFrame.cpp 
    src/App.cpp 
    "src/Frames/OptionFrames/CompressMultipleFrame.cpp" 
    "src/Frames/OptionFrames/CompressSingleFrame.cpp"
    "src/Frames/OptionFrames/ImageOptionFrame.cpp"
    "src/ImageManager.cpp" "src/Frames/OptionFrames/DisplayImageFrame.cpp" 
     
    "src/CustomWidgets/ListColorsPanel.cpp" "src/CustomWidgets/ImageDescriptionPanel.cpp"  "src/CustomWidgets/ImageDisplayPanel.cpp")

# Header files (for IDE organization)
set(HEADERS
    include/KClusterAlgorithm.h
    include/Constants.h
    include/Frames/MainFrame.h 
    include/App.h 
    "include/Frames/OptionFrames/CompressMultipleFrame.h" 
    "include/Frames/OptionFrames/CompressSingleFrame.h"
    "include/Frames/OptionFrames/ImageOptionFrame.h"
    "include/ImageManager.h" 
    "include/Frames/OptionFrames/DisplayImageFrame.h" 
     
    "include/CustomWidgets/ListColorsPanel.h"
 "include/CustomWidgets/ImageDescriptionPanel.h"  "include/CustomWidgets/ImageDisplayPanel.h")

# For relative access
add_compile_definitions(PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    wx::core 
    wx::base
    OpenMP::OpenMP_CXX  # OpenMP library
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Visual Studio specific settings
if(MSVC)
    # Set startup project for Visual Studio
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    
    # Set working directory for debugging
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
    
    # Enable parallel compilation
    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
    
    # Add OpenMP compile flag for MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE /openmp)
endif()

# GCC/Clang specific OpenMP settings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)
    target_link_options(${PROJECT_NAME} PRIVATE -fopenmp)
endif()

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

# Print useful information
message(STATUS "")
message(STATUS "=== BUILD CONFIGURATION ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenMP: ${OpenMP_CXX_FOUND}")
if(OpenMP_CXX_FOUND AND OpenMP_CXX_VERSION)
    message(STATUS "OpenMP version: ${OpenMP_CXX_VERSION}")
endif()
message(STATUS "wxWidgets: ${wxWidgets_FOUND}")
if(wxWidgets_FOUND)
    message(STATUS "wxWidgets source: vcpkg")
else()
    message(STATUS "wxWidgets source: Downloaded from GitHub")
endif()
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "===========================")
message(STATUS "")