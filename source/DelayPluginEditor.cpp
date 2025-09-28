#include "DelayPluginEditor.h"


DelayPluginEditor::DelayPluginEditor (DelayPluginProcessor& p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , gainKnob("Gain", processorRef.apvts, gainParamID)
    , mixKnob("Mix", processorRef.apvts, mixParamID)
    , delayTimeKnob("Delay Time", processorRef.apvts, delayTimeParamID)
{
    addAndMakeVisible (inspectButton);

    // allow host/user to resize and provide sensible limits so hosts know we can scale
    setResizable (true, true);
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

    delayGroup.setText("Delay");
    delayGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    delayGroup.addAndMakeVisible(delayTimeKnob);
    addAndMakeVisible(delayGroup);

    feedbackGroup.setText("Feedback");
    feedbackGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    addAndMakeVisible(feedbackGroup);

    outputGroup.setText("Output");
    outputGroup.setTextLabelPosition(juce::Justification::horizontallyCentred);
    outputGroup.addAndMakeVisible(gainKnob);
    outputGroup.addAndMakeVisible(mixKnob);
    addAndMakeVisible(outputGroup);

    // initial editor size
    setSize (500, 330);
}

DelayPluginEditor::~DelayPluginEditor()
{
}

void DelayPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void DelayPluginEditor::resized()
{
    auto bounds = getLocalBounds();

    int y = 10;
    int height = bounds.getHeight() - 20;

    delayGroup.setBounds(10, y, 110, height);
    outputGroup.setBounds(bounds.getWidth() - 160, y, 150, height);
    feedbackGroup.setBounds(delayGroup.getRight() + 10, y, outputGroup.getX() - delayGroup.getRight() - 20, height);

    delayTimeKnob.setTopLeftPosition(20, 20);
    mixKnob.setTopLeftPosition(20, 20);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);

}

