cmake_minimum_required(VERSION 3.16)
project(audiopanning LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Multimedia Widgets SpatialAudio)

find_package(PkgConfig REQUIRED)

pkg_check_modules(SNDFILE REQUIRED sndfile)

if(NOT SNDFILE_FOUND)
    message(FATAL_ERROR "libsndfile library not found. Please install libsndfile or check your environment.")
endif()

qt_add_executable(audiopanning
    main.cpp
    audiowidget.cpp
    soundvisualizationwidget.cpp
    audiowidget.h
    soundvisualizationwidget.h
)

target_link_libraries(audiopanning PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Multimedia
    Qt6::Widgets
    Qt6::SpatialAudio
    ${SNDFILE_LIBRARIES}
)

target_include_directories(audiopanning PRIVATE
    ${SNDFILE_INCLUDE_DIRS}
)

if(DEFINED SNDFILE_LIBRARY_DIRS)
    target_link_directories(audiopanning PRIVATE ${SNDFILE_LIBRARY_DIRS})
endif()

target_compile_options(audiopanning PRIVATE
    -Wall
    -Wextra
    -pedantic
)

install(TARGETS audiopanning
    RUNTIME DESTINATION "${INSTALL_EXAMPLESDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLESDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLESDIR}"
)
