#include "PluginProcessor.h"
#include "PluginEditor.h"

// c-style parameter init func

AudioProcessorValueTreeState::ParameterLayout initParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back(std::make_unique<AudioParameterFloat>(
        "volume", // parameter ID
        "Volume", // parameter name
        NormalisableRange<float>(-48.0f, 12.0f, 0.1f), // parameter range
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
    : apiCommunicationTimer(motuWebApi)
{
    motuWebApi.setTimeout(1); // dont wait for the calls to complete

    parameters = new AudioProcessorValueTreeState(*this, nullptr, Identifier("Faderoni"), initParameterLayout());

    for (int i = 0; i < 8; i++) {
        volumeParameters[i] = dynamic_cast<AudioParameterFloat*>(parameters->getParameter("volume_" + i));
        panningParameters[i] = dynamic_cast<AudioParameterInt*>(parameters->getParameter("panning_" + i));
        apiCommunicationTimer.setVolumeParameter(i, volumeParameters[i]);
        apiCommunicationTimer.setPanningParameter(i, panningParameters[i]);
    }

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
    ignoreUnused(layouts);
    return true;
}
#endif

void FaderoniAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
}

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
    auto state = parameters->copyState();
    const std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FaderoniAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    const std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState == nullptr)
        return;

    if (xmlState->hasTagName(parameters->state.getType()))
        parameters->replaceState(ValueTree::fromXml(*xmlState));

    hostnameParameter = parameters->state.getChildWithProperty("id", "hostname");

    if (!hostnameParameter.isValid())
    {
        hostnameParameter = ValueTree("PARAM");
        hostnameParameter.setProperty("id", "hostname", nullptr);
        hostnameParameter.setProperty("value", "motu", nullptr);
        parameters->state.appendChild(hostnameParameter, nullptr);
    }

    amountOfChannelsParameter = parameters->state.getChildWithProperty("id", "amount_of_channels");

    if (!amountOfChannelsParameter.isValid())
    {
        amountOfChannelsParameter = ValueTree("PARAM");
        amountOfChannelsParameter.setProperty("id", "amount_of_channels", nullptr);
        amountOfChannelsParameter.setProperty("value", "3", nullptr);
        parameters->state.appendChild(amountOfChannelsParameter, nullptr);
    }

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        subtreeParameters[i] = parameters->state.getChildWithProperty("id", "subtree_" + i);

        if (!subtreeParameters[i].isValid())
        {
            subtreeParameters[i] = ValueTree("PARAM");
            subtreeParameters[i].setProperty("id", "subtree_" + i, nullptr);
            subtreeParameters[i].setProperty("value", "mix/chan/0/matrix", nullptr);
            parameters->state.appendChild(subtreeParameters[i], nullptr);
        }
    }
}

void FaderoniAudioProcessor::setVolume(const int& channel, float volume)
{
    volumeParameters[channel]->setValueNotifyingHost(volumeParameters[channel]->convertTo0to1(volume));
}

void FaderoniAudioProcessor::setPanning(const int& channel, int panning)
{
    panningParameters[channel]->setValueNotifyingHost(panningParameters[channel]->convertTo0to1(panning));
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

void FaderoniAudioProcessor::setHost(const String& hostname)
{
    motuWebApi.setHostname(hostname);
    hostnameParameter.setProperty("value", hostname, nullptr);
}

void FaderoniAudioProcessor::setSubtree(const int& channel, const String& subtree)
{
    apiCommunicationTimer.setSubtree(channel, subtree);
    subtreeParameters[channel].setProperty("value", subtree, nullptr);
}

void FaderoniAudioProcessor::setAmountOfChannels(const int& amount)
{
    apiCommunicationTimer.setAmountOfChannels(amount);
    amountOfChannelsParameter.setProperty("value", String(amount), nullptr);
}
