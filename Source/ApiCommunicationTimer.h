#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FaderoniConstants.h"
#include "MotuWebApi.h"


class ApiCommunicationTimer : public Timer {

public:
    ApiCommunicationTimer(MotuWebApi&);

    void setAmountOfChannelsParameter(ValueTree* amountOfChannelsParameter);
    void setVolumeParameter(int channel, AudioParameterFloat* volume);
    void setPanningParameter(int channel, AudioParameterInt* panning);
    void setSubtreeParameter(int channel, ValueTree* subtree);

private:
    SpinLock syncRoot;

    ValueTree* amountOfChannelsParameter;
    AudioParameterFloat* volumes[FADERONI_MAX_CHANNELS]{};
    AudioParameterInt* pannings[FADERONI_MAX_CHANNELS]{};
    ValueTree* subtrees[FADERONI_MAX_CHANNELS];

    float prevVolumes[FADERONI_MAX_CHANNELS]{};
    float prevPannings[FADERONI_MAX_CHANNELS]{};

    MotuWebApi& motuWebApi;

    void timerCallback() override;
    static float transformPanningValueToMultiplicator(float value);
    static double transformVolumeValueToMultiplicator(float value);
};
