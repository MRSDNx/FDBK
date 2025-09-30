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

static const juce::ParameterID feedbackParamID { "feedback", 1 };
static constexpr const char* feedbackParamIDString = "feedback";

static const juce::ParameterID stereoParamID { "stereo", 1 };
static constexpr const char* stereoParamIDString = "stereo";

static const juce::ParameterID highcutParamID { "highcut", 1 };
static constexpr const char* highcutParamIDString = "highcut";

static const juce::ParameterID lowcutParamID { "lowcut", 1 };
static constexpr const char* lowcutParamIDString = "lowcut";

class Parameters
{
public:

    float delayTime = 0.05f;
    float feedback = 0.0f;
    float gain = 0.0f;
    float mix = 1.0f;
    float stereo = 0.0f;
    float highcut = 0.0f;
    float lowcut = 0.0f;

    float panL = 0.0f;
    float panR = 1.0f;

    static constexpr float minDelayTime = 5.0f;
    static constexpr float maxDelayTime = 5000.0f;

    explicit Parameters(juce::AudioProcessorValueTreeState& apvts);
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void smoothen() noexcept;
    void update() noexcept;

private:
    juce::AudioParameterFloat* gainParam { nullptr };
    juce::AudioParameterFloat* delayTimeParam { nullptr };
    juce::AudioParameterFloat* mixParam { nullptr };
    juce::AudioParameterFloat* feedbackParam { nullptr };
    juce::AudioParameterFloat* stereoParam { nullptr };
    juce::AudioParameterFloat* highcutParam { nullptr };
    juce::AudioParameterFloat* lowcutParam { nullptr };

    juce::LinearSmoothedValue<float> gainSmoother;
    juce::LinearSmoothedValue<float> mixSmoother;
    juce::LinearSmoothedValue<float> delayTimeSmoother;
    juce::LinearSmoothedValue<float> feedbackSmoother;
    juce::LinearSmoothedValue<float> stereoSmoother;
    juce::LinearSmoothedValue<float> highcutSmoother;
    juce::LinearSmoothedValue<float> lowcutSmoother;

    float targetDelayTime = 0.0f;
    float coeff = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Parameters)

};