#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SamplerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    SamplerAudioProcessorEditor (SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SamplerAudioProcessor& audioProcessor;
    juce::Slider volumeSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessorEditor)
};
