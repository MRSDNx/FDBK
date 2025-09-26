// Created by Benjahmin Singh-Reynolds on 9/24/25.

#include "Parameters.h"

// dynamic_cast helper function to keep shit clean
template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);
}

// dynamic_cast is now done in the castParameter function
Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f)
    {
        return juce::String(value, 2) + "ms";
    }

    else if (value < 100.0f)
    {
        return juce::String(value, 1) + "ms";
    }

    else if (value < 1000.0f)
    {
        return juce::String(int(value)) + "ms";
    }

    else
    {
        return juce::String(value * 0.001f, 2) + "s";
    }
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + "dB";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + "%";
}

// actually creates the gain slider
juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID, "Output Gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f }, 0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(delayTimeParamID, "Delay Time",
        juce::NormalisableRange<float> { minDelayTime, maxDelayTime, 0.001f, 0.25f}, 100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)));

    layout.add(std::make_unique<juce::AudioParameterFloat>(mixParamID, "Mix",
        juce::NormalisableRange<float> {0.0f, 100.0f, 1.0f}, 100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)));
        
    return layout;
}

// eliminates zippering while changing gain values via slider/knob
void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02; //20ms
    gainSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);// 48000 × 0.02 = 960

    // After 200ms (0.2f), the 1-pole filter will have approached the target value to within 63.2%.
    // Why? The formula below describes the charge time of an analog capacitor.
    // It is common to use these representations in DSP code, as a great deal of DSP is based on EE.
    // Using a 1-pole here for an exponential curve to avoid zipper noise since it fits better in context.

    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));
}

void Parameters::reset() noexcept
{
    delayTime = 0.0f;
    mix = 1.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
}

void Parameters::smoothen() noexcept
{
    // reads the currently smoothed value and puts it into the gain variable
    gain = gainSmoother.getNextValue();
    mix = mixSmoother.getNextValue();

    // the 1-pole filter formula
    delayTime += (targetDelayTime - delayTime) * coeff;
}

void Parameters::update() noexcept
{
    // Update the smoother's target when the parameter changes; the actual
    // gain value will be read via smoothen() per-sample to avoid zipper noise.
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setTargetValue(mixParam->get() * 0.01f);

    targetDelayTime = delayTimeParam->get();

    if (delayTime == 0.0f)
    {
        delayTime = targetDelayTime;
    }
}