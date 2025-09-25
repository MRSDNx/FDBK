// Created by Benjahmin Singh-Reynolds on 9/24/25.

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

// Parameter IDs
static const juce::ParameterID gainParamID { "gain", 1 };
static constexpr const char* gainParamIDString = "gain";

class Parameters
{
public:

    explicit Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    void update() noexcept;

    float gain = 0.0f;

private:
    juce::AudioParameterFloat* gainParam { nullptr };
    juce::LinearSmoothedValue<float> gainSmoother;
};