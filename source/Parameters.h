// Created by Benjahmin Singh-Reynolds on 9/24/25.

#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

// Parameter IDs
static const juce::ParameterID gainParamID { "gain", 1 };
static constexpr const char* gainParamIDString = "gain";

static const juce::ParameterID delayTimeParamID {"delayTime", 1 };
static constexpr const char* delayTimeParamIDString = "delayTime";

static const juce::ParameterID mixParamID {"mix", 1 };
static constexpr const char* mixParamIDString = "mix";

class Parameters
{
public:

    float delayTime = 0.05f;

    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;

    explicit Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    void update() noexcept;

    float gain = 0.0f;
    float mix = 1.0f;

private:
    juce::AudioParameterFloat* gainParam { nullptr };
    juce::AudioParameterFloat* delayTimeParam { nullptr };
    juce::AudioParameterFloat* mixParam { nullptr };

    juce::LinearSmoothedValue<float> gainSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;

    float targetDelayTime = 0.0f;
    float coeff = 0.0f;

};