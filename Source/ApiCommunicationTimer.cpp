#include "ApiCommunicationTimer.h"


//==============================================================================
ApiCommunicationTimer::ApiCommunicationTimer(MotuWebApi& motuWebApi)
    : motuWebApi(motuWebApi) { }

//==============================================================================
void ApiCommunicationTimer::setVolumeParameter(AudioParameterFloat* volume)
{
    this->volume = volume;
}

void ApiCommunicationTimer::setPanningParameter(AudioParameterInt* panning)
{
    this->panning = panning;
}

void ApiCommunicationTimer::setSubtree(const String& subtree)
{
    this->subtree = subtree;
}

void ApiCommunicationTimer::timerCallback()
{
    if (*volume != prevVolume)
    {
        prevVolume = *volume;
        motuWebApi.setVolume(subtree, transformVolumeValueToMultiplicator(prevVolume));
    }

    if (*panning != prevPanning)
    {
        prevPanning = *panning;
        motuWebApi.setPanning(subtree, transformPanningValueToMultiplicator(prevPanning));
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