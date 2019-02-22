/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FaderoniConstants.h"
#include "MotuWebApi.h"
#include "PluginEditor.h"
#include "APICommunicationTimer.h"

//==============================================================================
/**
*/
class FaderoniAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    FaderoniAudioProcessor();
    ~FaderoniAudioProcessor();

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void setVolume(const int& channel, float volume);
    void setPanning(const int& channel, int panning);
    void setMasterVolume(float volume);
    float transformVolumeValueToMultiplicator(float value) const;

    void setHost(const String& string);
    void setSubtree(const int& channel, const String& subtree);
    void setAmountOfChannels(const int& amount);
    void setChannelMode(const int& channel, bool mode);
private:
    MotuWebApi motuWebApi;
    ApiCommunicationTimer apiCommunicationTimer;

    AudioProcessorValueTreeState* parameters = nullptr;
    ValueTree hostnameParameter;
    ValueTree amountOfChannelsParameter;
    AudioParameterFloat* volumeParameters[FADERONI_MAX_CHANNELS];
    AudioParameterInt* panningParameters[FADERONI_MAX_CHANNELS];
    AudioParameterFloat* eqParameters[FADERONI_MAX_CHANNELS];
    AudioParameterFloat* masterVolumeParameter;
    ValueTree subtreeParameters[FADERONI_MAX_CHANNELS];
    ValueTree channelModeParameters[FADERONI_MAX_CHANNELS];

    FaderoniAudioProcessorEditor* editor;

    void initializeParameters();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessor);
};
