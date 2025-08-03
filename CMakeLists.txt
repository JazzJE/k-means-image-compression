cmake_minimum_required(VERSION 3.16)
project(k_means_cluster_image_compression)

# Set C++ standard to 17 for filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download and setup STB library (REQUIRED)
include(FetchContent)

message(STATUS "Downloading STB library...")
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(stb)

# Create interface library for STB
add_library(stb_lib INTERFACE)
target_include_directories(stb_lib INTERFACE ${stb_SOURCE_DIR})

# Verify STB files are available
if(NOT EXISTS ${stb_SOURCE_DIR}/stb_image_write.h)
    message(FATAL_ERROR "STB download failed - stb_image_write.h not found")
endif()

if(NOT EXISTS ${stb_SOURCE_DIR}/stb_image.h)
    message(FATAL_ERROR "STB download failed - stb_image.h not found")
endif()

message(STATUS "STB library successfully downloaded and configured")
message(STATUS "STB location: ${stb_SOURCE_DIR}")

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
    "src/Frames/CompressMultipleFrame.cpp" 
    "src/Frames/CompressSingleFrame.cpp"
    "src/Frames/BaseCompressFrame.cpp"
)

# Header files (for IDE organization)
set(HEADERS
    include/KClusterAlgorithm.h
    include/Constants.h
    include/Frames/MainFrame.h 
    include/App.h 
    "include/Frames/CompressMultipleFrame.h" 
    "include/Frames/CompressSingleFrame.h"
    "include/Frames/BaseCompressFrame.h"
)

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
    stb_lib  # STB library
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
    
    # Add STB files to Visual Studio solution for easy access
    target_sources(${PROJECT_NAME} PRIVATE 
        ${stb_SOURCE_DIR}/stb_image_write.h
        ${stb_SOURCE_DIR}/stb_image.h
    )
    
    # Group STB files in Visual Studio
    source_group("External\\STB" FILES 
        ${stb_SOURCE_DIR}/stb_image_write.h
        ${stb_SOURCE_DIR}/stb_image.h
    )
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
message(STATUS "wxWidgets: ${wxWidgets_FOUND}")
if(wxWidgets_FOUND)
    message(STATUS "wxWidgets source: vcpkg")
else()
    message(STATUS "wxWidgets source: Downloaded from GitHub")
endif()
message(STATUS "STB directory: ${stb_SOURCE_DIR}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "===========================")
message(STATUS "")

# Optional: Create a custom target to show library usage information
add_custom_target(library_info
    COMMAND ${CMAKE_COMMAND} -E echo "=== LIBRARY USAGE INFORMATION ==="
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "STB Usage in your C++ files:"
    COMMAND ${CMAKE_COMMAND} -E echo "#define STB_IMAGE_WRITE_IMPLEMENTATION"
    COMMAND ${CMAKE_COMMAND} -E echo "#include \"stb_image_write.h\""
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "#define STB_IMAGE_IMPLEMENTATION" 
    COMMAND ${CMAKE_COMMAND} -E echo "#include \"stb_image.h\""
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "wxWidgets Usage:"
    COMMAND ${CMAKE_COMMAND} -E echo "#include <wx/wx.h>"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "=================================="
    COMMENT "Showing library usage information"
)