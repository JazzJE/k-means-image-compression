cmake_minimum_required(VERSION 3.25...4.10)
project(k_means_cluster_image_compression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------
# OpenMP
# -------------------------------
find_package(OpenMP QUIET)
if(NOT OpenMP_FOUND OR NOT OpenMP_CXX_FOUND)
    message(STATUS "OpenMP not found, please install it manually")
else()
    message(STATUS "OpenMP found via system")
endif()

# -------------------------------
# wxWidgets
# -------------------------------
# Use system-installed wxWidgets first
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
    message(STATUS "wxWidgets found: ${wxWidgets_VERSION_STRING}")
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found. Install wxWidgets on your system (Linux: libwxgtk3.0-gtk3-dev, macOS: brew install wxwidgets, Windows: vcpkg)")
endif()

# -------------------------------
# Source and Header files
# -------------------------------
set(SOURCES
    src/KClusterAlgorithm.cpp
    src/Frames/MainFrame.cpp 
    src/App.cpp 
    src/Frames/OptionFrames/CompressMultipleFrame.cpp 
    src/Frames/OptionFrames/CompressSingleFrame.cpp
    src/Frames/OptionFrames/ImageOptionFrame.cpp
    src/ImageManager.cpp 
    src/Frames/OptionFrames/DisplayImageFrame.cpp 
    src/CustomWidgets/ListColorsPanel.cpp 
    src/CustomWidgets/ImageDescriptionPanel.cpp  
    src/CustomWidgets/ImageDisplayPanel.cpp
)

set(HEADERS
    include/KClusterAlgorithm.h
    include/Constants.h
    include/Frames/MainFrame.h 
    include/App.h 
    include/Frames/OptionFrames/CompressMultipleFrame.h 
    include/Frames/OptionFrames/CompressSingleFrame.h
    include/Frames/OptionFrames/ImageOptionFrame.h
    include/ImageManager.h 
    include/Frames/OptionFrames/DisplayImageFrame.h 
    include/CustomWidgets/ListColorsPanel.h
    include/CustomWidgets/ImageDescriptionPanel.h  
    include/CustomWidgets/ImageDisplayPanel.h
)

add_compile_definitions(PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${wxWidgets_LIBRARIES}
)

# OpenMP linking
if(OpenMP_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()

# Platform-specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
endif()

# -------------------------------
# Compiler-specific flags
# -------------------------------
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /MP /openmp)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fopenmp)
    target_link_options(${PROJECT_NAME} PRIVATE -fopenmp)
endif()

# -------------------------------
# Output directory
# -------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# -------------------------------
# Print configuration summary
# -------------------------------
message(STATUS "")
message(STATUS "=== BUILD CONFIGURATION ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenMP: ${OpenMP_FOUND}")
if(OpenMP_FOUND AND OpenMP_CXX_VERSION)
    message(STATUS "OpenMP version: ${OpenMP_CXX_VERSION}")
endif()
message(STATUS "wxWidgets: ${wxWidgets_FOUND} (${wxWidgets_VERSION_STRING})")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "===========================")
message(STATUS "")
