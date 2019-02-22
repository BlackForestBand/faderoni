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
    void setMode(const int&channel, bool mode);


    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;
    void setSize();
    void updateVisibility();

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
    TextEditor inputSubtrees[FADERONI_MAX_CHANNELS];

    TextButton btnModeVolume[FADERONI_MAX_CHANNELS];
    TextButton btnModeEq[FADERONI_MAX_CHANNELS];
    Label lblMode[FADERONI_MAX_CHANNELS];


    Slider sliderVolumes[FADERONI_MAX_CHANNELS];
    Slider sliderPannings[FADERONI_MAX_CHANNELS];
    Slider sliderEqs[FADERONI_MAX_CHANNELS];

    Slider sliderMasterVolume;

    std::unique_ptr<SliderAttachment> volumeAttachments[FADERONI_MAX_CHANNELS];
    std::unique_ptr<SliderAttachment> panningAttachments[FADERONI_MAX_CHANNELS];
    std::unique_ptr<SliderAttachment> masterVolumeAttachment;
    std::unique_ptr<SliderAttachment> eqAttachments[FADERONI_MAX_CHANNELS];
    int amountOfChannels = FADERONI_MAX_CHANNELS;
    bool channelMode[FADERONI_MAX_CHANNELS]{};
    
    LookAndFeel_V4 faderoniLook;
    Font headerFont;
    Font bodyFont;
    Colour accentColour;

    float prevEqs[FADERONI_MAX_CHANNELS]{};
    float prevVolumes[FADERONI_MAX_CHANNELS]{};
    int prevPannings[FADERONI_MAX_CHANNELS]{};
    float prevMasterVolume;
    bool prevMode[FADERONI_MAX_CHANNELS]{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessorEditor)
};
