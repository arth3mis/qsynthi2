#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "QSynthi2/Parameter/Parameter.h"
#include "QSynthi2/List/list.h"

// simple cout logger
//
class SimpleLogger : public juce::Logger {
public:
    ~SimpleLogger() override = default;
    SimpleLogger() = default;
protected:
    void logMessage(const juce::String &message) override {
        std::cout << message << std::endl;
    }
};


//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)

    std::shared_ptr<Parameter> parameter;

    List<double> benchmarkSamples = List<double>(1024);
    int benchmarkCounter = 0;
    float floatVariable = 0;
    double doubleVariable = 0;

    long total_0 = 0;
    long total_1 = 0;
    long total_2 = 0;
};