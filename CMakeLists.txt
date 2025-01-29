cmake_minimum_required(VERSION 3.16)
project(audiopanning LANGUAGES CXX)

# Enable Qt autogen features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia Widgets SpatialAudio)

# Try to find SoX and libsndfile using pkg-config
find_package(PkgConfig REQUIRED)

# Ensure SoX is found through pkg-config
pkg_check_modules(SOX REQUIRED sox)

# Ensure libsndfile is found through pkg-config
pkg_check_modules(SNDFILE REQUIRED sndfile)

# Check if the SoX libraries and include directories are found
if(NOT SOX_FOUND)
    message(FATAL_ERROR "SoX library not found. Please install SoX or check your environment.")
endif()

# Check if the libsndfile libraries and include directories are found
if(NOT SNDFILE_FOUND)
    message(FATAL_ERROR "libsndfile library not found. Please install libsndfile or check your environment.")
endif()

# Add source files and headers
qt_add_executable(audiopanning
    main.cpp
    audiowidget.cpp
    soundvisualizationwidget.cpp
    audiowidget.h
    soundvisualizationwidget.h
)

# Link Qt libraries, SoX libraries, and libsndfile
target_link_libraries(audiopanning PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Widgets
    Qt6::SpatialAudio
    ${SOX_LIBRARIES}
    ${SNDFILE_LIBRARIES}
)

# Include SoX and libsndfile headers in the target
target_include_directories(audiopanning PRIVATE ${SOX_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})

# If necessary, add the SoX and libsndfile library directory paths
if(DEFINED SOX_LIBRARY_DIRS)
    target_link_directories(audiopanning PRIVATE ${SOX_LIBRARY_DIRS})
endif()

if(DEFINED SNDFILE_LIBRARY_DIRS)
    target_link_directories(audiopanning PRIVATE ${SNDFILE_LIBRARY_DIRS})
endif()

# Enable compiler flags for better warnings and checks
target_compile_options(audiopanning PRIVATE
    -Wall
    -Wextra
    -pedantic
)

# Install the executable
install(TARGETS audiopanning
    RUNTIME DESTINATION "${INSTALL_EXAMPLESDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLESDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLESDIR}"
)
