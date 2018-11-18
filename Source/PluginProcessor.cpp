/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FaderoniAudioProcessor::FaderoniAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
    prevVolume = -1000;
    prevPanning = -1000;

    addParameter(volume = new AudioParameterFloat(
        "volume", // parameter ID
        "Volume", // parameter name
        NormalisableRange<float>(-48.0f, 12.0f), // parameter range
        0.0f));
    addParameter(panning = new AudioParameterInt(
        "panning", // parameter ID
        "Panning", // parameter name
        -100,
        100, // parameter range
        0));


    volume->addListener(this);
    panning->addListener(this);
}

FaderoniAudioProcessor::~FaderoniAudioProcessor()
{
}

//==============================================================================
const String FaderoniAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FaderoniAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool FaderoniAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool FaderoniAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double FaderoniAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FaderoniAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int FaderoniAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FaderoniAudioProcessor::setCurrentProgram(int index)
{
}

const String FaderoniAudioProcessor::getProgramName(int index)
{
    return {};
}

void FaderoniAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void FaderoniAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    prevVolume = *volume;
}

void FaderoniAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FaderoniAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void FaderoniAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
}


void FaderoniAudioProcessor::processVolume()
{
    const auto currentVolume = volume->get();
    const auto host = editor->inputHost.getText();

    if (currentVolume != prevVolume)
    {
        motuWebApi.setHostname(host);
        motuWebApi.setVolume(0, transformVolumeValueToMultiplicator(currentVolume));

        prevVolume = currentVolume;
    }
}

void FaderoniAudioProcessor::processPanning()
{
    const auto currentPanning = panning->get();

    if (currentPanning != prevPanning)
    {
        // send panning change to OSC

        prevPanning = currentPanning;
    }
}

//==============================================================================
bool FaderoniAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FaderoniAudioProcessor::createEditor()
{
    return (editor = new FaderoniAudioProcessorEditor(*this));
}

//==============================================================================
void FaderoniAudioProcessor::getStateInformation(MemoryBlock& destData)
{
}

void FaderoniAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void FaderoniAudioProcessor::setVolume(double volume)
{
    this->volume->setValueNotifyingHost(static_cast<float>(this->volume->convertTo0to1(volume)));
    processVolume();
}

void FaderoniAudioProcessor::setPanning(int panning)
{
    this->panning->setValueNotifyingHost(panning);
    processPanning();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FaderoniAudioProcessor();
}


double FaderoniAudioProcessor::transformVolumeValueToMultiplicator(float value) const
{
    if (value == -48)
        return 0;
    if (value == 0)
        return 1;
    if (value == 12)
        return 4;

    return std::pow(10.0, value / 20.0);
}


double FaderoniAudioProcessor::transformVolumeMultiplicatorToValue(int value) const
{
    if (value <= 0)
        return -48;
    if (value == 1)
        return 0;
    if (value == 4)
        return 12;

    return 20 * std::log10(value);
}

void FaderoniAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    processVolume();
    editor->setVolume(volume->convertFrom0to1(newValue));
}

void FaderoniAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
