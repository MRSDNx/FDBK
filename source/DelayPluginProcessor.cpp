#include "DelayPluginEditor.h"
#include "DelayPluginProcessor.h"
#include "ProtectYourEars.h"

//==============================================================================
DelayPluginProcessor::DelayPluginProcessor():

    AudioProcessor(BusesProperties()
       .withInput("Input", juce::AudioChannelSet::stereo(), true)
       .withOutput("Output", juce::AudioChannelSet::stereo(), true)
       ), params(apvts)
{
    lowCutFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highCutFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

DelayPluginProcessor::~DelayPluginProcessor()
{
}

//==============================================================================
const juce::String DelayPluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String DelayPluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void DelayPluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void DelayPluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    params.prepareToPlay(sampleRate);
    params.reset();
    tempo.reset();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32(samplesPerBlock); // type cast from the data type of samplesPerBlock. only holds +nums
    spec.numChannels = 2;

    double numSamples = Parameters::maxDelayTime / 1000.0 * sampleRate;
    int maxDelayInSamples = int(std::ceil(numSamples));

    delayLineL.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineR.setMaximumDelayInSamples(maxDelayInSamples);
    delayLineL.reset();
    delayLineR.reset();

    feedbackL = 0.0f;
    feedbackR = 0.0f;

    lastLowCut = -1.0f;
    lastHighCut = -1.0f;

    lowCutFilter.prepare(spec);
    lowCutFilter.reset();

    highCutFilter.prepare(spec);
    highCutFilter.reset();

    levelL.store(0.0f);
    levelR.store(0.0f);

    // DBG(maxDelayInSamples);
}

void DelayPluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool DelayPluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    /*
     * The host will call the isBusesLayoutSupported() function to see what kind of buses it supports.
     * The function is supposed to return true for any bus layout the plugin can handle.
     */

    const auto mono = juce::AudioChannelSet::mono();
    const auto stereo = juce::AudioChannelSet::stereo();
    const auto mainIn = layouts.getMainInputChannelSet();
    const auto mainOut = layouts.getMainOutputChannelSet();

    if (mainIn == mono && mainOut == mono) { return true; }
    if (mainIn == mono && mainOut == stereo) { return true; }
    if (mainIn == stereo && mainOut == stereo) { return true; }

    return false;
}

void DelayPluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, [[maybe_unused]]
                                              juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    params.update();
    tempo.update(getPlayHead());

    float syncedTime = float(tempo.getMillisecondsFromNoteLength(params.delayNote));
    if (syncedTime > Parameters::maxDelayTime)
    {
        syncedTime = Parameters::maxDelayTime;
    }

    float sampleRate = float(getSampleRate());

    // logic for mono vs stereo assignment
    auto mainInput = getBusBuffer(buffer, true, 0);
    auto mainInputChannels = mainInput.getNumChannels();
    auto isMainInputStereo = mainInputChannels > 1;
    const float* inputDataL = mainInput.getReadPointer(0);
    const float* inputDataR = mainInput.getReadPointer(isMainInputStereo ? 1 : 0);

    auto mainOutput = getBusBuffer(buffer, false, 0);
    auto mainOutputChannels = mainOutput.getNumChannels();
    auto isMainOutputStereo = mainOutputChannels > 1;
    float* outputDataL = mainOutput.getWritePointer(0);
    float* outputDataR = mainOutput.getWritePointer(isMainOutputStereo ? 1 : 0);

    float maxL = 0.0f;
    float maxR = 0.0f;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        params.smoothen();

        float delayTime = params.tempoSync ? syncedTime : params.delayTime;
        float delayInSamples = params.delayTime * 0.001f * sampleRate;

        if (params.lowcut != lastLowCut)
        {
            lowCutFilter.setCutoffFrequency(params.lowcut);
            lastLowCut = params.lowcut;
        }

        if (params.highcut != lastHighCut)
        {
            highCutFilter.setCutoffFrequency(params.highcut);
            lastHighCut = params.highcut;
        }

        float dryL = inputDataL[sample];
        float dryR = inputDataR[sample];

        float mono = (dryL + dryR) * 0.5f;

        delayLineL.write(mono*params.panL + feedbackL);
        delayLineR.write(mono*params.panR + feedbackR);

        float wetL = delayLineL.read(delayInSamples);
        float wetR = delayLineR.read(delayInSamples);

        feedbackL = wetL * params.feedback;
        feedbackL = lowCutFilter.processSample(0, feedbackL);
        feedbackL = highCutFilter.processSample(0, feedbackL);

        feedbackR = wetR * params.feedback;
        feedbackR = lowCutFilter.processSample(1, feedbackR);
        feedbackR = highCutFilter.processSample(1, feedbackR);

        float mixL = dryL + wetL * params.mix;
        float mixR = dryR + wetR * params.mix;

        float outL = mixL * params.gain;
        float outR = mixR * params.gain;

        outputDataL[sample] = outL;
        outputDataR[sample] = outR;

        maxL = std::max(maxL, std::abs(outL));
        maxR = std::max(maxR, std::abs(outR));
    }

    levelL.store(maxL);
    levelR.store(maxR);

#if JUCE_DEBUG
    protectYourEars(buffer);
#endif
}

//==============================================================================
bool DelayPluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayPluginProcessor::createEditor()
{
    return new DelayPluginEditor (*this);
}

//==============================================================================
void DelayPluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void DelayPluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }

    // DBG(apvts.copyState().toXmlString());
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginProcessor();
}


