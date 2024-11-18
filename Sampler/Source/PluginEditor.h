#pragma once

#include <JuceHeader.h>
#include "CubeVisualizer.h"
#include "PluginProcessor.h"

class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::FileDragAndDropTarget,
                                    private juce::Slider::Listener
{
public:
    SamplerAudioProcessorEditor(SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    SamplerAudioProcessor& audioProcessor;

    // Declare the vertical panning slider
    juce::Slider verticalPanSlider;
    juce::Label verticalPanLabel;
    juce::Slider volumeSlider, panSlider;
    juce::ListBox sampleList;
    CubeVisualizer cubeVisualizer;
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};
