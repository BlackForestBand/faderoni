/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FaderoniConstants.h"
#include "PluginProcessor.h"

class FaderoniAudioProcessor;
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

//==============================================================================
/**
*/
class FaderoniAudioProcessorEditor : public AudioProcessorEditor
{
public:
    FaderoniAudioProcessorEditor(FaderoniAudioProcessor&, AudioProcessorValueTreeState* parameters);
    ~FaderoniAudioProcessorEditor();

    void setVolume(const int& channel, const float& val);
    void setPanning(const int& channel, const int& val);
    void setAmountOfChannels(const int& amount);
    void setMasterVolume(const float& val);

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void setSize();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FaderoniAudioProcessor& processor;
    AudioProcessorValueTreeState* parameters;

    Label lblMasterVolume;
    Label lblTitle;
    Label lblHost;
    Label lblAmountOfChannels;
    Label lblSubtrees[FADERONI_MAX_CHANNELS];

    TextButton btnMinus, btnPlus;

    TextEditor inputHost;
    TextEditor inputSubtrees[8];

    Slider sliderVolumes[FADERONI_MAX_CHANNELS];
    Slider sliderPannings[FADERONI_MAX_CHANNELS];

    Slider sliderMasterVolume;

    std::unique_ptr<SliderAttachment> volumeAttachments[FADERONI_MAX_CHANNELS];
    std::unique_ptr<SliderAttachment> panningAttachments[FADERONI_MAX_CHANNELS];
    std::unique_ptr<SliderAttachment> masterVolumeAttachment;
    int amountOfChannels = FADERONI_MAX_CHANNELS;

    LookAndFeel_V4 faderoniLook;
    Font headerFont;
    Font bodyFont;
    Colour accentColour;

    float prevVolumes[8]{};
    int prevPannings[8]{};
    float prevMasterVolume;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessorEditor)
};
