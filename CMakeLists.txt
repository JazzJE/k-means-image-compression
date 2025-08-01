cmake_minimum_required(VERSION 3.16)
project(k_means_cluster_image_compression)

# Set C++ standard to 17 for filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets (using vcpkg)
find_package(wxWidgets CONFIG REQUIRED)

# Source files
set(SOURCES
    src/KClusterAlgorithm.cpp
    src/Cluster.cpp
    src/Frames/MainFrame.cpp 
    src/App.cpp 
    "src/Frames/CompressMultipleFrame.cpp" 
    "src/Frames/CompressSingleFrame.cpp"
 "src/Frames/BaseCompressFrame.cpp")

# Header files (for IDE organization)
set(HEADERS
    include/KClusterAlgorithm.h
    include/Constants.h
    include/Frames/MainFrame.h 
    include/App.h 
    "include/Frames/CompressMultipleFrame.h" 
    "include/Frames/CompressSingleFrame.h"
 "include/Frames/BaseCompressFrame.h")

# For relative access
add_compile_definitions(PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link wxWidgets libraries (vcpkg style)
target_link_libraries(${PROJECT_NAME} wx::core wx::base)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

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