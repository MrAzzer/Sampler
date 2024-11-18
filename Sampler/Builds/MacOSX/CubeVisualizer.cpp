#include "CubeVisualizer.h"

CubeVisualizer::CubeVisualizer()
{
    formatManager.registerBasicFormats();  // Registers common audio formats (WAV, MP3, AIFF)
}

CubeVisualizer::~CubeVisualizer() {}

void CubeVisualizer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    // Add your custom painting code here (e.g., drawing the waveform)
}

void CubeVisualizer::resized() {}

void CubeVisualizer::addSource(const juce::String& filePath)
{
    juce::File file(filePath);
    auto* reader = formatManager.createReaderFor(file);

    if (reader != nullptr)
    {
        // If the file is valid, store the reader or process the audio
        audioReader = reader;
    }
}
