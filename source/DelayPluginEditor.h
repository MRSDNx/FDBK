#pragma once

#include "BinaryData.h"
#include "DelayPluginProcessor.h"
#include "melatonin_inspector/melatonin_inspector.h"

//==============================================================================
class DelayPluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit DelayPluginEditor (DelayPluginProcessor&);
    ~DelayPluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayPluginProcessor& processorRef;
    std::unique_ptr<melatonin::Inspector> inspector;
    juce::TextButton inspectButton { "Inspect the UI" };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginEditor)
};
