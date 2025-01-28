# Sampler
Cpp oop QT (formerly with JUCE)
This is a simple QT project for standalone binaural audio plugin.

# Build
Open the project folder in terminal and type:
$ rm -rf build
$ mkdir build
$ cd build
$ cmake ..
$ make

and run with:
$ ./audiopanning

# Workflow
The plugin is a 3 segment system app. The left panel is where the data files go. You can paste the path in the search bar or simply drag 'n' drop the files to the file tray. From here, you select the file to apply binaural effects on. Once the spatial audio parameters are set, you can playback the audio file or export them with applied effects.

# Architecture
The plugin was meant to be built in JUCE but I lack expertiese knowledge with C++ frameworks. Because of QT's built in system audio processing,I managed to use it's convolution rather than building it from scratch in JUCE. 
