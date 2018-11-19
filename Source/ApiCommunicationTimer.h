#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MotuWebApi.h"


class ApiCommunicationTimer : public Timer {

public:
    ApiCommunicationTimer(MotuWebApi);

    void setVolumeParameter(AudioParameterFloat* volume);
    void setPanningParameter(AudioParameterInt* panning);

private:
    AudioParameterFloat* volume = nullptr;
    AudioParameterInt* panning = nullptr;

    float prevVolume;
    float prevPanning;

    MotuWebApi motuWebApi;

    void timerCallback() override;
};
