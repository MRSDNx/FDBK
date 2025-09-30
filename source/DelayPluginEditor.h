#pragma once

#include "BinaryData.h"
#include "DelayPluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class DelayPluginEditor : public juce::AudioProcessorEditor
{
public:
    DelayPluginEditor (DelayPluginProcessor&);
    ~DelayPluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    MainLookAndFeel mainLF;
    juce::GroupComponent delayGroup, feedbackGroup, outputGroup;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginProcessor& processorRef;

    // the ui components for these are init on the constructor
    RotaryKnob gainKnob;
    RotaryKnob mixKnob;
    RotaryKnob delayTimeKnob;
    RotaryKnob feedbackKnob;
    RotaryKnob stereoKnob;
    RotaryKnob highcutKnob;
    RotaryKnob lowcutKnob;

    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginEditor)
};
