#include "DelayPluginEditor.h"

DelayPluginEditor::DelayPluginEditor (DelayPluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (inspectButton);

    // allow host/user to resize and provide sensible limits so hosts know we can scale
    setResizable (true, false);
    setResizeLimits (300, 200, 2000, 2000);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // initial editor size
    setSize (600, 400);
}

DelayPluginEditor::~DelayPluginEditor()
{
}

void DelayPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font(40.0f));
    g.drawFittedText ("FDBK v1.0", getLocalBounds(), juce::Justification::centred, 1);
}

void DelayPluginEditor::resized()
{
    // layout child components relative to current size so they scale with the editor
    auto area = getLocalBounds();
    auto buttonSize = juce::Rectangle<int> {}.withSize (juce::jmin (area.getWidth(), 200), 40);
    inspectButton.setBounds (area.withSizeKeepingCentre (buttonSize.getWidth(), buttonSize.getHeight()));
}

