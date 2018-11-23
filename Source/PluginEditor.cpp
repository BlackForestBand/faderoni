/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <dshow.h>

//==============================================================================
FaderoniAudioProcessorEditor::FaderoniAudioProcessorEditor(FaderoniAudioProcessor& p, AudioProcessorValueTreeState* parameters)
    : AudioProcessorEditor(&p), processor(p), faderoniLook(LookAndFeel_V4::getMidnightColourScheme()),
    headerFont(30, Font::bold), bodyFont(15), accentColour(0, 200, 255), parameters(parameters)
{
    faderoniLook.setColour(Slider::trackColourId, accentColour);
    faderoniLook.setColour(Slider::thumbColourId, accentColour);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);

    lblTitle.setFont(headerFont);
    lblTitle.setText("Faderoni", dontSendNotification);

    lblHost.setFont(bodyFont);
    lblHost.setText("API-Host:", dontSendNotification);
    lblOscPath.setFont(bodyFont);
    lblOscPath.setText("Path:", dontSendNotification);

    inputHost.onTextChange = [this]() { processor.setHost(inputHost.getText()); };
    inputHost.setText("motu", true);

    inputSubtree.onTextChange = [this]() { processor.setSubtree(inputSubtree.getText()); };
    inputSubtree.setText("mix/chan/0/matrix", true);

    btnSend.setButtonText("SEND");
    btnSend.setLookAndFeel(&faderoniLook);

    // these define the parameters of our volume slider object
    sliderVolume.setSliderStyle(Slider::LinearVertical);
    sliderVolume.setRange(-48, 12, 0.1);
    sliderVolume.setPopupDisplayEnabled(true, false, this);
    sliderVolume.setLookAndFeel(&faderoniLook);
    sliderVolume.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
    sliderVolume.textFromValueFunction = [this](const double val) { return transformVolumeValueToText(val); };
    sliderVolume.valueFromTextFunction = [this](const String text) { return transformVolumeTextToValue(text); };
    sliderVolume.setDoubleClickReturnValue(true, 102);
    sliderVolume.setValue(0);

    // these define the parameters of our slider object
    sliderPanning.setSliderStyle(Slider::RotaryHorizontalDrag);
    sliderPanning.setRange(-100, 100, 1);
    sliderPanning.setPopupDisplayEnabled(true, false, this);
    sliderPanning.setLookAndFeel(&faderoniLook);
    sliderPanning.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
    sliderPanning.textFromValueFunction = [this](const double val) { return transformPanningValueToText(val); };
    sliderPanning.valueFromTextFunction = [this](const String text) { return transformPanningTextToValue(text); };
    sliderPanning.setDoubleClickReturnValue(true, 64);
    sliderPanning.setValue(0);


    // this function adds the slider to the editor
    addAndMakeVisible(&lblTitle);
    addAndMakeVisible(&lblHost);
    addAndMakeVisible(&lblOscPath);
    addAndMakeVisible(&inputHost);
    addAndMakeVisible(&inputSubtree);
    addAndMakeVisible(&btnSend);
    addAndMakeVisible(&sliderVolume);
    addAndMakeVisible(&sliderPanning);

    // slider attachments for automation
    volumeAttachment.reset(new SliderAttachment(*parameters, "volume", sliderVolume));
    panningAttachment.reset(new SliderAttachment(*parameters, "panning", sliderPanning));
}

FaderoniAudioProcessorEditor::~FaderoniAudioProcessorEditor()
{
    btnSend.setLookAndFeel(nullptr);
    sliderVolume.setLookAndFeel(nullptr);
    sliderPanning.setLookAndFeel(nullptr);
}

//==============================================================================
void FaderoniAudioProcessorEditor::setVolume(float val)
{
    if (val == prevVolume)
        return;

    prevVolume = val;
    sliderVolume.setValue(val, dontSendNotification);
}

void FaderoniAudioProcessorEditor::setPanning(int val)
{
    if (val == prevPanning)
        return;

    prevPanning = val;
    sliderPanning.setValue(val, dontSendNotification);
}

//==============================================================================
void FaderoniAudioProcessorEditor::paint(Graphics& g)
{
    // fill the whole window white
    g.fillAll(Colour(20, 20, 20));
    // set the current drawing colour to black
    /*g.setColour(Colours::white);
    // set the font size and draw text to the screen
    //g.setFont(headerFont);
    g.setFont(24.0f);

    g.drawFittedText("Faderoni", 100, 22, getWidth(), 30, Justification::left, 1);
    */
}

void FaderoniAudioProcessorEditor::resized()
{
    const auto height = getHeight();
    static const auto padX = 40;
    static const auto padY = 30;
    static const auto inputHeight = 20;

    lblTitle.setBounds(120, padY, 150, 40);

    lblHost.setBounds(120, padY + 50, 100, inputHeight);
    lblOscPath.setBounds(120, padY + 80, 100, inputHeight);

    inputHost.setBounds(220, padY + 50, 150, inputHeight);
    inputSubtree.setBounds(220, padY + 80, 150, inputHeight);

    btnSend.setBounds(250, padY + 170, 120, height - 170 - 2 * padY);

    sliderVolume.setBounds(padX, padY, 60, height - 2 * padY);
    sliderPanning.setBounds(120, height - 100 - padY, 100, 100);
}

int FaderoniAudioProcessorEditor::transformPanningTextToValue(String text) const
{
    if (text.equalsIgnoreCase("L"))
        return -100;
    if (text.equalsIgnoreCase("C"))
        return 0;
    if (text.equalsIgnoreCase("R"))
        return 100;

    try
    {
        return std::stoi(text.toStdString());
    }
    catch (std::invalid_argument ex) { return 0; }
    catch (std::out_of_range ex) { return 0; }
}

int FaderoniAudioProcessorEditor::transformVolumeTextToValue(String text) const
{
    try
    {
        return std::stoi(text.toStdString());
    }
    catch (std::invalid_argument ex) { return 102; }
    catch (std::out_of_range ex) { return 102; }
}

String FaderoniAudioProcessorEditor::transformPanningValueToText(int value) const
{
    if (value == -100)
        return String("<L>");
    if (value == 0)
        return String("<C>");
    if (value == 100)
        return String("<R>");

    if (value < 0)
        return "L" + String(value);

    return "R" + String(value);
}

String FaderoniAudioProcessorEditor::transformVolumeValueToText(float value) const
{
    String volumeString;

    if (value == -48)
        volumeString = "-inf";
    else
    {
        if (value > 0)
            volumeString = "+" + String(value);
        else
            volumeString = String(value);
    }

    return volumeString + " dB";
}
