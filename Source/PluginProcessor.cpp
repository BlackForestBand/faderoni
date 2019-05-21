#include "PluginProcessor.h"
#include "PluginEditor.h"

// c-style parameter init func

int transformPanningTextToValue(const String& text);
int transformVolumeTextToValue(const String& text);
String transformPanningValueToText(int value);
String transformVolumeValueToText(float value);
int transformEqualizerTextToValue(const String& text);
String transformEqualizerFrequencyValueToText(float value);

AudioProcessorValueTreeState::ParameterLayout initParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    NormalisableRange<float> equalizerFrequencyParameterRange = NormalisableRange<float>(20.0f, 20000.0f, 1.0f);
    equalizerFrequencyParameterRange.setSkewForCentre(632);

    params.push_back(std::make_unique<AudioParameterFloat>(
        "master_volume", //paramter ID
        "Master Volume",
        NormalisableRange<float>(-48.0f, 12.0f, 0.1f),
        0.0f,
        "Master Volume",
        AudioProcessorParameter::genericParameter,
        [](const float val, const int maximumStringLength) {return transformVolumeValueToText(val);  },
        [](const String text) {return transformVolumeTextToValue(text); }));

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        params.push_back(std::make_unique<AudioParameterFloat>(
            "volume_" + String(i), // parameter ID
            "Volume " + String(i), // parameter name
            NormalisableRange<float>(-48.0f, 12.0f, 0.1f), // parameter range
            0.0f,
            "Volume " + String(i),
            AudioProcessorParameter::genericParameter,
            [](const float val, const int maximumStringLength) { return transformVolumeValueToText(val); },
            [](const String text) { return transformVolumeTextToValue(text); }));
        params.push_back(std::make_unique<AudioParameterInt>(
            "panning_" + String(i), // parameter ID
            "Panning " + String(i), // parameter name
            -100,
            100, // parameter range
            0,
            "Panning " + String(i),
            [](const int val, const int maximumStringLength) { return transformPanningValueToText(val); },
            [](const String text) { return transformPanningTextToValue(text); }));
        params.push_back(std::make_unique<AudioParameterFloat>(
            "eq_" + String(i),
            "EQ Frequency " + String(i),
            equalizerFrequencyParameterRange,
            2000.0f,
            "EQ Frequency " + String(i),
            AudioProcessorParameter::genericParameter,
            [](const int val, const int maximumStringLength) { return transformEqualizerFrequencyValueToText(val); },
            [](const String text) { return transformEqualizerTextToValue(text); }
        ));
    }


    return { params.begin(), params.end() };
}

//==============================================================================
FaderoniAudioProcessor::FaderoniAudioProcessor()
    : apiCommunicationTimer(motuWebApi)
{
    parameters = new AudioProcessorValueTreeState(*this, nullptr, Identifier("Faderoni"), initParameterLayout());
    initializeParameters();

    apiCommunicationTimer.setAmountOfChannelsParameter(&amountOfChannelsParameter);

    masterVolumeParameter = dynamic_cast<AudioParameterFloat*>(parameters->getParameter("master_volume"));
    apiCommunicationTimer.setMasterVolumeParameter(masterVolumeParameter);


    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        volumeParameters[i] = dynamic_cast<AudioParameterFloat*>(parameters->getParameter("volume_" + String(i)));
        panningParameters[i] = dynamic_cast<AudioParameterInt*>(parameters->getParameter("panning_" + String(i)));
        equalizerFrequencyParameters[i] = dynamic_cast<AudioParameterFloat*>(parameters->getParameter("eq_" + String(i)));
        apiCommunicationTimer.setVolumeParameter(i, volumeParameters[i]);
        apiCommunicationTimer.setPanningParameter(i, panningParameters[i]);
        apiCommunicationTimer.setSubtreeParameter(i, &subtreeParameters[i]);
        apiCommunicationTimer.setEqParameter(i, equalizerFrequencyParameters[i]);
    }

    motuWebApi.setTimeout(1); // dont wait for the calls to complete

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

    initializeParameters();
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

    return std::pow(10.0f, value / 20.0f);
}

void FaderoniAudioProcessor::setHost(const String& hostname)
{
    motuWebApi.setHostname(hostname);
    hostnameParameter.setProperty("value", hostname, nullptr);
}

void FaderoniAudioProcessor::setSubtree(const int& channel, const String& subtree)
{
    subtreeParameters[channel].setProperty("value", subtree, nullptr);
}

void FaderoniAudioProcessor::setAmountOfChannels(const int& amount)
{
    amountOfChannelsParameter.setProperty("value", String(amount), nullptr);
}

void FaderoniAudioProcessor::setChannelMode(const int& channel, bool mode)
{
    channelModeParameters[channel].setProperty("value", mode, nullptr);
}


void FaderoniAudioProcessor::initializeParameters()
{
    hostnameParameter = parameters->state.getChildWithProperty("id", "hostname");
    if (!hostnameParameter.isValid())
    {
        hostnameParameter = ValueTree("PARAM");
        hostnameParameter.setProperty("id", "hostname", nullptr);
        hostnameParameter.setProperty("value", "motu", nullptr);
        parameters->state.appendChild(hostnameParameter, nullptr);
    }

    motuWebApi.setHostname(hostnameParameter.getPropertyAsValue("value", nullptr).getValue());

    amountOfChannelsParameter = parameters->state.getChildWithProperty("id", "amount_of_channels");
    if (!amountOfChannelsParameter.isValid())
    {
        amountOfChannelsParameter = ValueTree("PARAM");
        amountOfChannelsParameter.setProperty("id", "amount_of_channels", nullptr);
        amountOfChannelsParameter.setProperty("value", 3, nullptr);
        parameters->state.appendChild(amountOfChannelsParameter, nullptr);
    }

    apiCommunicationTimer.setAmountOfChannelsParameter(&amountOfChannelsParameter);


    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        subtreeParameters[i] = parameters->state.getChildWithProperty("id", "subtree_" + String(i));

        if (!subtreeParameters[i].isValid())
        {
            subtreeParameters[i] = ValueTree("PARAM");
            subtreeParameters[i].setProperty("id", "subtree_" + String(i), nullptr);
            subtreeParameters[i].setProperty("value", "mix/chan/" + String(i) + "/matrix", nullptr);
            parameters->state.appendChild(subtreeParameters[i], nullptr);
        }

        apiCommunicationTimer.setSubtreeParameter(i, &subtreeParameters[i]);

        channelModeParameters[i] = parameters->state.getChildWithProperty("id", "channelMode_" + String(i));
        if (!channelModeParameters[i].isValid())
        {
            channelModeParameters[i] = ValueTree("PARAM");
            channelModeParameters[i].setProperty("id", "channelMode_" + String(i), nullptr);
            channelModeParameters[i].setProperty("value", false, nullptr);
            parameters->state.appendChild(channelModeParameters[i], nullptr);
        }
        apiCommunicationTimer.setChannelModeParameter(i, &channelModeParameters[i]);
    }
}


int transformPanningTextToValue(const String& text)
{
    if (text.equalsIgnoreCase("L"))
        return -100;
    if (text.equalsIgnoreCase("C"))
        return 0;
    if (text.equalsIgnoreCase("R"))
        return 100;

    try
    {
        return std::stoi(text.toStdString());
    }
    catch (std::invalid_argument ex) { return 0; }
    catch (std::out_of_range ex) { return 0; }
}

int transformVolumeTextToValue(const String& text)
{
    try
    {
        return std::stoi(text.toStdString());
    }
    catch (std::invalid_argument ex) { return -48; }
    catch (std::out_of_range ex) { return -48; }
}

String transformEqualizerFrequencyValueToText(float value)
{
    return String(floor(value)) + " Hz";
}

int transformEqualizerTextToValue(const String& text)
{
    try
    {
        return floor(std::stoi(text.toStdString()));
    }
    catch (std::invalid_argument ex) { return 20; }
    catch (std::out_of_range ex) { return 20; }
}

String transformPanningValueToText(int value)
{
    if (value == -100)
        return String("<L>");
    if (value == 0)
        return String("<C>");
    if (value == 100)
        return String("<R>");

    if (value < 0)
        return "L" + String(-value);

    return "R" + String(value);
}

String transformVolumeValueToText(float value)
{
    String volumeString;

    if (value == -48)
        volumeString = "-inf";
    else
    {
        if (value > 0)
            volumeString = "+" + String(value, 1);
        else
            volumeString = String(value, 1);
    }

    return volumeString + " dB";
}
