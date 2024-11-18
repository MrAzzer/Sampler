#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor & Destructor
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor(SamplerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Volume Slider
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    volumeSlider.onValueChange = [this] { audioProcessor.setVolume(volumeSlider.getValue()); };
    addAndMakeVisible(volumeSlider);

    // Pan Slider
    panSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    panSlider.setRange(-1.0, 1.0);
    panSlider.setValue(0.0);
    panSlider.onValueChange = [this] { audioProcessor.setPan(panSlider.getValue()); };
    addAndMakeVisible(panSlider);

    // In the constructor of the editor
    verticalPanSlider.setSliderStyle(juce::Slider::LinearVertical);
    verticalPanSlider.setRange(-1.0f, 1.0f, 0.01f);  // The range allows for values from -1 to +1
    verticalPanSlider.setValue(0.0f);  // Default to center (middle)
    verticalPanSlider.onValueChange = [this] { audioProcessor.setVerticalPan(verticalPanSlider.getValue()); };
    addAndMakeVisible(verticalPanSlider);

    // Label for the vertical pan slider
    verticalPanLabel.setText("Vertical Pan", juce::dontSendNotification);
    verticalPanLabel.attachToComponent(&verticalPanSlider, false);
    addAndMakeVisible(verticalPanLabel);

    // Cube Visualizer (Ensure this is defined and correctly sized)
    addAndMakeVisible(cubeVisualizer);

    setSize(800, 600);  // Default window size
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor() {}

void SamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::blue);  // Background color
}

void SamplerAudioProcessorEditor::resized()
{
    // Set bounds for the sliders and the cube visualizer
    volumeSlider.setBounds(10, 10, 50, 300);
    panSlider.setBounds(70, getHeight() - 40, 150, 30);
    verticalPanSlider.setBounds(50, 50, 20, 200);  // Proper position for vertical pan slider
    cubeVisualizer.setBounds(230, 10, getWidth() - 240, getHeight() - 20);  // Adjust based on the window size
}

bool SamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    return files[0].endsWith(".wav") || files[0].endsWith(".mp3") || files[0].endsWith(".aiff");
}

void SamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    if (files[0].endsWith(".wav"))
    {
        // Handle file drop logic here (e.g., load the file)
    }
}

void SamplerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        audioProcessor.setVolume(volumeSlider.getValue());
    }
    else if (slider == &panSlider)
    {
        audioProcessor.setPan(panSlider.getValue());
    }
    else if (slider == &verticalPanSlider)
    {
        audioProcessor.setVerticalPan(verticalPanSlider.getValue());
    }
}
