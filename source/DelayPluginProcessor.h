#pragma once
#include "Parameters.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "Tempo.h"
#include "DelayLine.h"

#if (MSVC)
#include "ipps.h"
#endif

class DelayPluginProcessor : public juce::AudioProcessor
{
public:

    juce::AudioProcessorValueTreeState apvts{*this, nullptr, "Parameters", Parameters::createParameterLayout() };

    DelayPluginProcessor();
    ~DelayPluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Parameters params;

    std::atomic<float> levelL, levelR;

private:


    Tempo tempo;

    float feedbackL = 0.0f;
    float feedbackR = 0.0f;

    float lastLowCut = -1.0f;
    float lastHighCut = -1.0f;

    DelayLine delayLineL, delayLineR;
    juce::dsp::StateVariableTPTFilter<float> lowCutFilter;
    juce::dsp::StateVariableTPTFilter<float> highCutFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginProcessor)
};
