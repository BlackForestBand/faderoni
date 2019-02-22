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

void ApiCommunicationTimer::setEqParameter(int channel, AudioParameterFloat* eq)
{
    eqs[channel] = eq;
}


void ApiCommunicationTimer::setPanningParameter(int channel, AudioParameterInt* panning)
{
    pannings[channel] = panning;
}

void ApiCommunicationTimer::setSubtreeParameter(int channel, ValueTree* subtree)
{
    subtrees[channel] = subtree;
}

void ApiCommunicationTimer::setChannelModeParameter(int channel, juce::ValueTree* mode)
{
    channelModes[channel] = mode;
}


void ApiCommunicationTimer::setMasterVolumeParameter(AudioParameterFloat* mV)
{
    masterVolume = mV;
}


void ApiCommunicationTimer::timerCallback()
{
    const auto amountOfChannels = static_cast<int>(amountOfChannelsParameter->getPropertyAsValue("value", nullptr).getValue());
    bool refreshVolume = false;
    if (*masterVolume != prevMasterVolume)
    {
        prevMasterVolume = *masterVolume;
        refreshVolume = true;
    }

    for (auto i = 0; i < amountOfChannels; i++) {
        const bool mode = channelModes[i]->getPropertyAsValue("value", nullptr).getValue();
        bool mode_switched = false;
        if(mode != prevMode[i])
        {
            prevMode[i] = mode;
            mode_switched = true;
        }
            

        if(mode)
        {
            if(*eqs[i] != prevEq[i] || mode_switched)
            {    
                prevEq[i] = *eqs[i];
                motuWebApi.setEq(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), prevEq[i]);
            }
            
        } else
        {
            if ((*volumes[i] != prevVolumes[i]) || refreshVolume || mode_switched)
            {
                prevVolumes[i] = *volumes[i];
                motuWebApi.setVolume(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), transformVolumeValueToMultiplicator(addVolumes(prevVolumes[i], prevMasterVolume)));
            }

            if (*pannings[i] != prevPannings[i] || mode_switched)
            {
                prevPannings[i] = *pannings[i];
                motuWebApi.setPanning(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), transformPanningValueToMultiplicator(prevPannings[i]));
            }
        }
        
    }
}

float ApiCommunicationTimer::transformPanningValueToMultiplicator(float value)
{
    return value / 100.0;
}

float ApiCommunicationTimer::addVolumes(float v1, float v2)
{    
    std::max(std::min(v1+v2,12.0f),48.0f);
    float val = v1 + v2;
    if (val > 12.0f)
        return 12.0f;
    if (val < -48.0f)
        return -48.0f;
    return val;
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