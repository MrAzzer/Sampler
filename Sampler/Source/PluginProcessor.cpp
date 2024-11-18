#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Constructor & Destructor
SamplerAudioProcessor::SamplerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo())
        .withOutput("Output", juce::AudioChannelSet::stereo())
    )
{
}

SamplerAudioProcessor::~SamplerAudioProcessor() {}

//==============================================================================
// Audio Processing
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}

void SamplerAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {}
void SamplerAudioProcessor::releaseResources() {}

bool SamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    auto currentVolume = volume.load();
    auto currentPan = pan.load();
    auto currentVerticalPan = verticalPan.load();  // Get vertical pan value

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Apply the regular volume
            float pannedValue = channelData[sample] * currentVolume;

            // Apply horizontal (left-right) panning
            if (channel == 0)  // Left
                pannedValue *= (1.0f - currentPan) * 0.5f;
            else if (channel == 1)  // Right
                pannedValue *= (1.0f + currentPan) * 0.5f;

            // Apply vertical (front-back) panning with frequency manipulation
            if (currentVerticalPan < 0.0f)  // Bottom (negative values)
            {
                // Emphasize low frequencies for "bottom" (increase bass presence)
                pannedValue *= (1.0f + currentVerticalPan * 0.25f);  // Increase presence for bottom sounds
            }
            else if (currentVerticalPan > 0.0f)  // Top (positive values)
            {
                // Emphasize high frequencies for "top" (reduce bass, increase treble presence)
                pannedValue *= (1.0f - currentVerticalPan * 0.25f);  // Decrease bass for top sounds
            }

            channelData[sample] = pannedValue;
        }
    }
}

void SamplerAudioProcessor::setVolume(float newVolume) { volume.store(newVolume); }
float SamplerAudioProcessor::getVolume() const { return volume.load(); }
void SamplerAudioProcessor::setPan(float newPan) { pan.store(newPan); }
void SamplerAudioProcessor::setVerticalPan(float newPan) { verticalPan.store(newPan); }

//==============================================================================
// Plugin Methods
const juce::String SamplerAudioProcessor::getName() const { return "Sampler"; }

bool SamplerAudioProcessor::acceptsMidi() const { return true; }
bool SamplerAudioProcessor::producesMidi() const { return false; }
bool SamplerAudioProcessor::isMidiEffect() const { return false; }
double SamplerAudioProcessor::getTailLengthSeconds() const { return 0.0; }

int SamplerAudioProcessor::getNumPrograms() { return 1; }
int SamplerAudioProcessor::getCurrentProgram() { return 0; }
void SamplerAudioProcessor::setCurrentProgram(int index) { /* No-op for single program */ }
const juce::String SamplerAudioProcessor::getProgramName(int index) { return "Default Program"; }
void SamplerAudioProcessor::changeProgramName(int index, const juce::String& newName) { /* No-op for single program */ }

void SamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Save state (e.g., volume, pan, vertical pan)
}

void SamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore state (e.g., volume, pan, vertical pan)
}

//==============================================================================
// Plugin Editor
bool SamplerAudioProcessor::hasEditor() const { return true; }

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor(*this);
}
