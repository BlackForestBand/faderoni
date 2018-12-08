#include "ApiCommunicationTimer.h"


//==============================================================================
ApiCommunicationTimer::ApiCommunicationTimer(MotuWebApi& motuWebApi)
    : motuWebApi(motuWebApi)
{
}

//==============================================================================
void ApiCommunicationTimer::setAmountOfChannelsParameter(ValueTree* amountOfChannelsParameter)
{
    this->amountOfChannelsParameter = amountOfChannelsParameter;
}

void ApiCommunicationTimer::setVolumeParameter(int channel, AudioParameterFloat* volume)
{
    volumes[channel] = volume;
}

void ApiCommunicationTimer::setPanningParameter(int channel, AudioParameterInt* panning)
{
    pannings[channel] = panning;
}

void ApiCommunicationTimer::setSubtreeParameter(int channel, ValueTree* subtree)
{
    subtrees[channel] = subtree;
}

void ApiCommunicationTimer::timerCallback()
{
    const auto amountOfChannels = static_cast<int>(amountOfChannelsParameter->getPropertyAsValue("value", nullptr).getValue());

    for (auto i = 0; i < amountOfChannels; i++) {
        if (*volumes[i] != prevVolumes[i])
        {
            prevVolumes[i] = *volumes[i];
            motuWebApi.setVolume(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), transformVolumeValueToMultiplicator(prevVolumes[i]));
        }

        if (*pannings[i] != prevPannings[i])
        {
            prevPannings[i] = *pannings[i];
            motuWebApi.setPanning(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), transformPanningValueToMultiplicator(prevPannings[i]));
        }
    }
}

float ApiCommunicationTimer::transformPanningValueToMultiplicator(float value)
{
    return value / 100.0;
}


double ApiCommunicationTimer::transformVolumeValueToMultiplicator(float value)
{
    if (value == -48)
        return 0;
    if (value == 0)
        return 1;
    if (value == 12)
        return 4;

    return std::pow(10.0, value / 20.0);
}