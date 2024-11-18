#include "PluginEditor.h"

SamplerAudioProcessorEditor::SamplerAudioProcessorEditor(SamplerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Volume Slider
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5);
    addAndMakeVisible(volumeSlider);
    volumeSlider.onValueChange = [this] { audioProcessor.setVolume(volumeSlider.getValue()); };

    // Pan Slider
    panSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    panSlider.setRange(-1.0, 1.0);
    panSlider.setValue(0.0);
    addAndMakeVisible(panSlider);
    panSlider.onValueChange = [this] { audioProcessor.setPan(panSlider.getValue()); };

    // Cube Visualizer
    addAndMakeVisible(cubeVisualizer);

    setSize(800, 600);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor() {}

void SamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void SamplerAudioProcessorEditor::resized()
{
    volumeSlider.setBounds(10, 10, 50, 300);
    panSlider.setBounds(70, getHeight() - 40, 150, 30);
    cubeVisualizer.setBounds(230, 10, getWidth() - 240, getHeight() - 20);
}

bool SamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (const auto& file : files)
        if (juce::File(file).hasFileExtension("wav;mp3;aiff"))
            return true;
    return false;
}

void SamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (const auto& file : files)
        cubeVisualizer.addSource(file);
}
