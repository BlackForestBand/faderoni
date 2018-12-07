#include "ApiCommunicationTimer.h"


//==============================================================================
ApiCommunicationTimer::ApiCommunicationTimer(MotuWebApi& motuWebApi)
    : motuWebApi(motuWebApi)
{
}

//==============================================================================
void ApiCommunicationTimer::setVolumeParameter(int channel, AudioParameterFloat* volume)
{
    this->volumes[channel] = volume;
}

void ApiCommunicationTimer::setPanningParameter(int channel, AudioParameterInt* panning)
{
    this->pannings[channel] = panning;
}

void ApiCommunicationTimer::setSubtree(int channel, const String& subtree)
{
    this->subtrees[channel] = subtree;
}

void ApiCommunicationTimer::setAmountOfChannels(const int& amount)
{
    this->amountOfchannels = amount;
}

void ApiCommunicationTimer::timerCallback()
{
    for (auto i = 0; i < amountOfchannels; i++) {
        if (*volumes[i] != prevVolumes[i])
        {
            prevVolumes[i] = *volumes[i];
            motuWebApi.setVolume(subtrees[i], transformVolumeValueToMultiplicator(prevVolumes[i]));
        }

        if (*pannings[i] != prevPannings[i])
        {
            prevPannings[i] = *pannings[i];
            motuWebApi.setPanning(subtrees[i], transformPanningValueToMultiplicator(prevPannings[i]));
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