#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MotuWebApi.h"


class ApiCommunicationTimer : public Timer {

public:
    ApiCommunicationTimer(MotuWebApi&);

    void setVolumeParameter(AudioParameterFloat* volume);
    void setPanningParameter(AudioParameterInt* panning);
    void setSubtree(const String& subtree);

private:
    AudioParameterFloat* volume = nullptr;
    AudioParameterInt* panning = nullptr;
    String subtree;

    float prevVolume;
    float prevPanning;

    MotuWebApi& motuWebApi;

    void timerCallback() override;
    static float transformPanningValueToMultiplicator(int value);
    static double transformVolumeValueToMultiplicator(int value);
};
