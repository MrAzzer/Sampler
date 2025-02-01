# Sampler
Cpp oop QT (formerly with JUCE)
This is a simple QT project for standalone binaural audio plugin.

# Installing prerequisites
You need to install Qt5 and CMake.

On Ubuntu:
$ sudo apt update
$ sudo apt install build-essential cmake pkg-config libsndfile1-dev qt6-base-dev qt6-multimedia-dev qt6-spatialaudio-dev

On Mac:
$ brew update
$ brew install cmake pkg-config libsndfile qt

On Windows:
https://www.qt.io/download-open-source

# Build
Open the project folder in terminal and type:
$ rm -rf build
$ mkdir build
$ cd build
$ cmake ..
$ make

and run with:
$ ./audiopanning

