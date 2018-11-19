#include "ApiCommunicationTimer.h"

ApiCommunicationTimer::ApiCommunicationTimer(MotuWebApi motuWebApi)
{
    this->motuWebApi = motuWebApi;
}

void ApiCommunicationTimer::setVolumeParameter(AudioParameterFloat* volume)
{
    this->volume = volume;
}

void ApiCommunicationTimer::setPanningParameter(AudioParameterInt* panning)
{
    this->panning = panning;
}

void ApiCommunicationTimer::timerCallback()
{
    if (*volume != prevVolume)
    {
        prevVolume = *volume;
    }

    if (*panning != prevPanning)
    {
        prevPanning = *panning;
    }
}
