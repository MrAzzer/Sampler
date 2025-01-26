cmake_minimum_required(VERSION 3.16)
project(audiopanning LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia Widgets SpatialAudio)

# Add source files and headers
qt_add_executable(audiopanning
    main.cpp
    audiowidget.cpp
    soundvisualizationwidget.cpp
)

# Link required Qt libraries
target_link_libraries(audiopanning PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Widgets
    Qt6::SpatialAudio
)

# Install the executable
install(TARGETS audiopanning
    RUNTIME DESTINATION "${INSTALL_EXAMPLESDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLESDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLESDIR}"
)
