#include "DelayPluginEditor.h"

DelayPluginEditor::DelayPluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
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
    // this is a button. layout the positions of your child components here

    //auto area = getLocalBounds();
    //area.removeFromBottom(50);
    //inspectButton.setBounds (getLocalBounds().withSizeKeepingCentre(100, 50));
}

