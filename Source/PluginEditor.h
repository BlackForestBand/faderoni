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

//==============================================================================
/**
*/
class FaderoniAudioProcessorEditor : public AudioProcessorEditor,
    public Slider::Listener
{
public:
    FaderoniAudioProcessorEditor(FaderoniAudioProcessor&);
    ~FaderoniAudioProcessorEditor();

    TextEditor inputHost;
    TextEditor inputOscPath;

    void setVolume(float val);

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FaderoniAudioProcessor& processor;

    Label lblTitle;
    Label lblHost;
    Label lblOscPath;
    TextButton btnSend;
    Slider sliderVolume;
    Slider sliderPanning;

    LookAndFeel_V4 faderoniLook;
    Font headerFont;
    Font bodyFont;
    Colour accentColour;

    float prevVolume;

    String transformPanningValueToText(int midiValue) const;
    String transformVolumeValueToText(float value) const;
    int transformPanningTextToValue(String text) const;
    int transformVolumeTextToValue(String text) const;

    void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FaderoniAudioProcessorEditor)
};
