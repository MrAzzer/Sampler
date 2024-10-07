#include <iostream>

using namespace std;

class AudioFile {
public:
    AudioFile(const string& filePath);
    bool load();
    void play();
    void stop();
    void setVolume(float volume);
    float getDuration() const;
    // Add other audio manipulation functions as needed

private:
    string filePath_;
    // Add members for audio data, volume, etc.
};

class Sampler {
public:
    void addAudioFile(const AudioFile& audioFile);
    void mergeAudioFiles(const string& outputFilePath);
    void playAll();
    void stopAll();
    void setGlobalVolume(float volume);

private:
    vector<AudioFile> audioFiles_;
    // Additional properties for mixing/merging
};

class AudioEngine {
public:
    AudioEngine();
    bool initialize();
    void startPlayback(const Sampler& sampler);
    void stopPlayback();
    // Other audio engine functionalities

private:
    // Members for the audio library context
};

int main(){
    AudioEngine audioEngine;
        audioEngine.initialize();

        Sampler sampler;

        // Load audio files
        AudioFile file1("/Users/spikespiegel/Documents/Cymatics - Apex One Shots Vol. 3/Cymatics - KEYS Rodeo - C.wav");
        AudioFile file2("/Users/spikespiegel/Documents/Cymatics - Apex One Shots Vol. 3/Cymatics - KEYS Rodeo - C.wav");

        if (file1.load() && file2.load()) {
            sampler.addAudioFile(file1);
            sampler.addAudioFile(file2);

            sampler.mergeAudioFiles("output.wav");
            sampler.playAll();
        }

        // Clean up and exit
        return 0;
}
