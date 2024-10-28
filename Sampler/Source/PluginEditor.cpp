#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Initialize the volume slider
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setRange(0.0, 1.0); // Set range from 0 to 1
    volumeSlider.setValue(0.5); // Default value
    volumeSlider.addListener(this); // Add this editor as a listener
    addAndMakeVisible(volumeSlider); // Add the slider to the editor

    setSize (400, 300); // Set the size of the editor
}
SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
    // Clean-up if needed, typically not needed if no resources are allocated.
}

void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Fill the background with a solid color
    g.fillAll (juce::Colour (0xff202020)); // Example background color
}

void SamplerAudioProcessorEditor::resized()
{
    // Set the bounds of the volume slider
    volumeSlider.setBounds (40, 40, 100, 200); // Example position and size
}

void SamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setVolume(volumeSlider.getValue());  // Use the public setter
    }
}
