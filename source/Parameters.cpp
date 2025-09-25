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
}

// actually creates the gain slider
juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID, "Output Gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f }, 0.0f));

    return layout;
}

// eliminates zippering while changing gain values via slider/knob
void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02; //20ms
    gainSmoother.reset(sampleRate, duration); // 48000 × 0.02 = 960
}

// tells the smoother what the gain parameter value is
// instead of re-initializing it to zero every time
void Parameters::reset() noexcept
{
    // read right to left
    // reads the current value from the parameter,
    // converts from decibels to linear units
    // tells the smoother about the new value
    // smoother gets to work

    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters::smoothen() noexcept
{
    // reads the currently smoothed value and puts it into the gain variable
    gain = gainSmoother.getNextValue();
}