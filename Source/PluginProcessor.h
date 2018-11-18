/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MotuWebApi.h"
#include "PluginEditor.h"

//==============================================================================
/**
*/
class FaderoniAudioProcessor : public AudioProcessor, public AudioProcessorParameter::Listener
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

    void setVolume(double volume);
    void setPanning(int panning);
    double transformVolumeValueToMultiplicator(float value) const;
    double transformVolumeMultiplicatorToValue(int value) const;

    void parameterValueChanged(int parameterIndex, float newValue) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override;

private:
    AudioParameterFloat* volume;
    AudioParameterInt* panning;
    float prevVolume;
    int prevPanning;

    FaderoniAudioProcessorEditor* editor;

    MotuWebApi motuWebApi;

    void processVolume();
    void processPanning();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessor)
};
