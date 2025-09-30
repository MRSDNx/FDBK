#include "DelayPluginEditor.h"


DelayPluginEditor::DelayPluginEditor (DelayPluginProcessor& p)
    : AudioProcessorEditor (&p)
    , processorRef (p)
    , gainKnob("Gain", processorRef.apvts, gainParamID, true)
    , mixKnob("Mix", processorRef.apvts, mixParamID)
    , delayTimeKnob("Delay Time", processorRef.apvts, delayTimeParamID)
    , feedbackKnob("Feedback", processorRef.apvts, feedbackParamID)
    , stereoKnob("Stereo", processorRef.apvts, stereoParamID)
    , highcutKnob("Highcut", processorRef.apvts, highcutParamID)
    , lowcutKnob("Lowcut", processorRef.apvts, lowcutParamID)
{
    setLookAndFeel (&mainLF);

    addAndMakeVisible (inspectButton);
    gainKnob.slider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::green);

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
    feedbackGroup.addAndMakeVisible(feedbackKnob);
    feedbackGroup.addAndMakeVisible(stereoKnob);
    feedbackGroup.addAndMakeVisible(lowcutKnob);
    feedbackGroup.addAndMakeVisible(highcutKnob);
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
    setLookAndFeel (nullptr);
}

void DelayPluginEditor::paint (juce::Graphics& g)

{
    auto noise = juce::ImageCache::getFromMemory(BinaryData::Noise_png, BinaryData::Noise_pngSize);
    auto fillType = juce::FillType(noise, juce::AffineTransform::scale(0.5f));
    g.setFillType(fillType);
    g.fillRect(getLocalBounds());

    auto rect = getLocalBounds().withHeight(40);
    g.setColour(Colors::header);
    g.fillRect(rect);

    auto image = juce::ImageCache::getFromMemory(BinaryData::Logo_png, BinaryData::Logo_pngSize);

    int destWidth = image.getWidth() / 2;
    int destHeight = image.getHeight() / 2;

    g.drawImage(image, getWidth() / 2 - destWidth / 2, 0, destWidth, destHeight, 0, 0,
        image.getWidth(), image.getHeight());

}

void DelayPluginEditor::resized()

{
    auto bounds = getLocalBounds();

    int y = 50;
    int height = bounds.getHeight() - 60;

    delayGroup.setBounds(10, y, 110, height);
    outputGroup.setBounds(bounds.getWidth() - 160, y, 150, height);
    feedbackGroup.setBounds(delayGroup.getRight() + 10, y, outputGroup.getX() - delayGroup.getRight() - 20, height);

    delayTimeKnob.setTopLeftPosition(20, 20);
    mixKnob.setTopLeftPosition(20, 20);
    gainKnob.setTopLeftPosition(mixKnob.getX(), mixKnob.getBottom() + 10);
    feedbackKnob.setTopLeftPosition(20, 20);
    stereoKnob.setTopLeftPosition(feedbackKnob.getRight() + 20, 20);
    highcutKnob.setTopLeftPosition(stereoKnob.getX(), stereoKnob.getBottom() + 10);
    lowcutKnob.setTopLeftPosition(feedbackKnob.getX(), feedbackKnob.getBottom() + 10);

}

