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

void ApiCommunicationTimer::setMasterVolumeParameter(AudioParameterFloat* mV)
{
	masterVolume = mV;
}


void ApiCommunicationTimer::timerCallback()
{
    const auto amountOfChannels = static_cast<int>(amountOfChannelsParameter->getPropertyAsValue("value", nullptr).getValue());
	bool refreshVolume = false;
	if(*masterVolume != prevMasterVolume)
	{
		prevMasterVolume = *masterVolume;
		refreshVolume = true;
	}

    for (auto i = 0; i < amountOfChannels; i++) {
        if ((*volumes[i] != prevVolumes[i]) || refreshVolume)
        {
            prevVolumes[i] = *volumes[i];
			motuWebApi.setVolume(subtrees[i]->getPropertyAsValue("value", nullptr).getValue(), transformVolumeValueToMultiplicator(addVolumes(prevVolumes[i], prevMasterVolume)));
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

float ApiCommunicationTimer::addVolumes(float v1, float v2)
{
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