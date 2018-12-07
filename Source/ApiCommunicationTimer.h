#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FaderoniConstants.h"
#include "MotuWebApi.h"


class ApiCommunicationTimer : public Timer {

public:
    ApiCommunicationTimer(MotuWebApi&);

    void setVolumeParameter(int channel, AudioParameterFloat* volume);
    void setPanningParameter(int channel, AudioParameterInt* panning);
    void setSubtree(int channel, String subtree);
    void setAmountOfChannels(const int& amount);

private:
    AudioParameterFloat* volumes[FADERONI_MAX_CHANNELS]{};
    AudioParameterInt* pannings[FADERONI_MAX_CHANNELS]{};
    String subtrees[8];

    float prevVolumes[FADERONI_MAX_CHANNELS]{};
    float prevPannings[FADERONI_MAX_CHANNELS]{};

    MotuWebApi& motuWebApi;
    int amountOfchannels = 0;

    void timerCallback() override;
    static float transformPanningValueToMultiplicator(float value);
    static double transformVolumeValueToMultiplicator(float value);
};
