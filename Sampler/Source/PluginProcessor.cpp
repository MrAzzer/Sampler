#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor & Destructor
SamplerAudioProcessor::SamplerAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                     .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
{
}

SamplerAudioProcessor::~SamplerAudioProcessor() {}

//==============================================================================
// Basic Info
const juce::String SamplerAudioProcessor::getName() const { return JucePlugin_Name; }

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const { return 0.0; }

//==============================================================================
// Program Management
int SamplerAudioProcessor::getNumPrograms() { return 1; }
int SamplerAudioProcessor::getCurrentProgram() { return 0; }
void SamplerAudioProcessor::setCurrentProgram(int) {}
const juce::String SamplerAudioProcessor::getProgramName(int) { return {}; }
void SamplerAudioProcessor::changeProgramName(int, const juce::String&) {}

//==============================================================================
// Audio Processing
void SamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
void SamplerAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void SamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto currentVolume = volume.load();
    auto currentPan = pan.load();

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float pannedValue = channelData[sample] * currentVolume;

            // Apply stereo panning
            if (channel == 0)  // Left
                pannedValue *= (1.0f - currentPan) * 0.5f;
            else if (channel == 1)  // Right
                pannedValue *= (1.0f + currentPan) * 0.5f;

            channelData[sample] = pannedValue;
        }
    }
}

//==============================================================================
// Plugin Editor
bool SamplerAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor(*this);
}

//==============================================================================
// State Management
void SamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {}
void SamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {}

//==============================================================================
// Factory Function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}

void SamplerAudioProcessor::setVolume(float newVolume) { volume.store(newVolume); }
void SamplerAudioProcessor::setPan(float newPan) { pan.store(newPan); }
