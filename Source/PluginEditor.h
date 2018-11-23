/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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

    TextEditor inputHost;
    TextEditor inputSubtree;

    void setVolume(float val);
    void setPanning(int val);

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FaderoniAudioProcessor& processor;
    AudioProcessorValueTreeState* parameters;

    Label lblTitle;
    Label lblHost;
    Label lblOscPath;
    TextButton btnSend;
    Slider sliderVolume;
    Slider sliderPanning;
    std::unique_ptr<SliderAttachment> volumeAttachment = nullptr;
    std::unique_ptr<SliderAttachment> panningAttachment = nullptr;

    LookAndFeel_V4 faderoniLook;
    Font headerFont;
    Font bodyFont;
    Colour accentColour;

    float prevVolume;
    int prevPanning;

    String transformPanningValueToText(int midiValue) const;
    String transformVolumeValueToText(float value) const;
    int transformPanningTextToValue(String text) const;
    int transformVolumeTextToValue(String text) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessorEditor)
};
