cmake_minimum_required(VERSION 3.15)
project(k_means_cluster_image_compression)

# Set C++ standard to 17 for filesystem
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
    src/KClusterAlgorithm.cpp
    src/Cluster.cpp
    src/main_helpers.cpp
)

# Header files (for IDE organization)
set(HEADERS
    include/KClusterAlgorithm.h
    include/Constants.h
    include/main_helpers.h
)

# For relative access
add_compile_definitions(PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
