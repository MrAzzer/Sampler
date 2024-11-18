#pragma once
#include <JuceHeader.h>

class CubeVisualizer : public juce::Component
{
public:
    CubeVisualizer();
    ~CubeVisualizer() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void addSource(const juce::String& filePath);

private:
    juce::AudioFormatManager formatManager;
    juce::AudioFormatReader* audioReader = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CubeVisualizer)
};
