#pragma once

#include <JuceHeader.h>

class SamplerAudioProcessor : public juce::AudioProcessor
{
public:
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter manipulation
    void setVolume(float newVolume);
    void setPan(float newPan);

private:
    // DSP Components
    std::atomic<float> volume{ 1.0f };    // Atomic for thread safety
    std::atomic<float> pan{ 0.0f };      // Range -1.0 (left) to 1.0 (right)

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessor)
};
