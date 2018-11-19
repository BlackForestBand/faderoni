/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioProcessorValueTreeState::ParameterLayout initParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(
        "volume", // parameter ID
        "Volume", // parameter name
        NormalisableRange<float>(-48.0f, 12.0f), // parameter range
        0.0f));
    params.push_back(std::make_unique<AudioParameterInt>(
        "panning", // parameter ID
        "Panning", // parameter name
        -100,
        100, // parameter range
        0));

    return { params.begin(), params.end() };
}

//==============================================================================
FaderoniAudioProcessor::FaderoniAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties())
#endif
{
    parameters = new AudioProcessorValueTreeState(*this, nullptr, Identifier("Faderoni"), initParameterLayout());
    
    volumeParameter = dynamic_cast<AudioParameterFloat*>(parameters->getParameter("volume"));
    panningParameter = dynamic_cast<AudioParameterInt*>(parameters->getParameter("panning"));

    apiCommunicationTimer.setVolumeParameter(volumeParameter);
    apiCommunicationTimer.setPanningParameter(panningParameter);


    parameters->addParameterListener("volume", this);
    parameters->addParameterListener("panning", this);

    apiCommunicationTimer.startTimerHz(5);
}

FaderoniAudioProcessor::~FaderoniAudioProcessor()
{
    delete parameters;
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

/*
void FaderoniAudioProcessor::processVolume(float volume)
{
    editor->setVolume(volume);
}

void FaderoniAudioProcessor::processPanning(int panning)
{
    editor->setPanning(panning);
}*/

//==============================================================================
bool FaderoniAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FaderoniAudioProcessor::createEditor()
{
    return (editor = new FaderoniAudioProcessorEditor(*this, parameters));
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

void FaderoniAudioProcessor::setVolume(float volume)
{
    volumeParameter->setValueNotifyingHost(volumeParameter->convertTo0to1(volume));
}

void FaderoniAudioProcessor::setPanning(int panning)
{
    panningParameter->setValueNotifyingHost(panningParameter->convertTo0to1(panning));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FaderoniAudioProcessor();
}


float FaderoniAudioProcessor::transformVolumeValueToMultiplicator(float value) const
{
    if (value == -48)
        return 0;
    if (value == 0)
        return 1;
    if (value == 12)
        return 4;

    return std::pow(10.0, value / 20.0);
}

float FaderoniAudioProcessor::transformPanningValueToMultiplicator(int value) const
{
    return value / 100.0;
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

void FaderoniAudioProcessor::parameterChanged(const String& parameterID, float newValue)
{
    if (parameterID == "volume")
        editor->setVolume(newValue);
    else if (parameterID == "panning")
        editor->setPanning(newValue);
}

/*
void FaderoniAudioProcessor::parameterValueChanged(int parameterIndex, float newValue)
{
    if (parameterIndex == 0)
        editor->setVolume(parameters->convertFrom0to1(newValue));
    else if (parameterIndex == 1)
        editor->setPanning(panning->convertFrom0to1(newValue));
}

void FaderoniAudioProcessor::parameterGestureChanged(int parameterIndex, bool gestureIsStarting)
{
}
*/
