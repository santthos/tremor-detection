cmake_minimum_required(VERSION 3.10)
project(TremorDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenCV REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OpenCV_INCLUDE_DIRS}
)

# Add Python module
add_subdirectory(src/python)

# Add C++ source files
set(CPP_SOURCES
    src/cpp/tremor_processor.cpp
    src/qt/mainwindow.cpp
    src/qt/main.cpp
)

# Add header files
set(HEADER_FILES
    include/tremor_processor.h
    include/mainwindow.h
)

# Create executable
add_executable(tremor_detection_gui
    ${CPP_SOURCES}
    ${HEADER_FILES}
)

# Link libraries
target_link_libraries(tremor_detection_gui
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Install MATLAB script
install(FILES src/matlab/tremor_analysis.m
    DESTINATION share/tremor_detection/matlab
)

# Install Python module
install(FILES src/python/video_capture.py
    DESTINATION share/tremor_detection/python
) 