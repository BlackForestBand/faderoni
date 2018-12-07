/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <dshow.h>
#include <math.h>

//==============================================================================
FaderoniAudioProcessorEditor::FaderoniAudioProcessorEditor(FaderoniAudioProcessor& p, AudioProcessorValueTreeState* parameters)
    : AudioProcessorEditor(&p), processor(p), faderoniLook(LookAndFeel_V4::getMidnightColourScheme()),
    headerFont(30, Font::bold), bodyFont(15), accentColour(0, 200, 255), parameters(parameters)
{
    amountOfChannels = parameters->getParameterAsValue("amount_of_channels").getValue();

    faderoniLook.setColour(Slider::trackColourId, accentColour);
    faderoniLook.setColour(Slider::thumbColourId, accentColour);


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    lblTitle.setFont(headerFont);
    lblTitle.setText("Faderoni", dontSendNotification);

    lblHost.setFont(bodyFont);
    lblHost.setText("API-Host:", dontSendNotification);

    inputHost.onTextChange = [this]() { processor.setHost(inputHost.getText()); };
    inputHost.setText(parameters->getParameterAsValue("hostname").getValue(), false);

    addAndMakeVisible(&lblTitle);
    addAndMakeVisible(&lblHost);
    addAndMakeVisible(&inputHost);


    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        lblSubtrees[i].setFont(bodyFont);
        lblSubtrees[i].setText("Path:", dontSendNotification);

        inputSubtrees[i].onTextChange = [this, i]() { processor.setSubtree(i, inputSubtrees[i].getText()); };
        inputSubtrees[i].setText(parameters->getParameterAsValue("subtree_" + String(i)).getValue(), false);

        // these define the parameters of our volume slider object
        sliderVolumes[i].setSliderStyle(Slider::LinearVertical);
        sliderVolumes[i].setRange(-48, 12, 0.1);
        sliderVolumes[i].setPopupDisplayEnabled(true, false, this);
        sliderVolumes[i].setLookAndFeel(&faderoniLook);
        sliderVolumes[i].setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 80, 20);
        sliderVolumes[i].textFromValueFunction = [this](const double val) { return transformVolumeValueToText(val); };
        sliderVolumes[i].valueFromTextFunction = [this](const String text) { return transformVolumeTextToValue(text); };
        sliderVolumes[i].setDoubleClickReturnValue(true, 102);
        sliderVolumes[i].setValue(0);

        // these define the parameters of our slider object
        sliderPannings[i].setSliderStyle(Slider::RotaryHorizontalDrag);
        sliderPannings[i].setRange(-100, 100, 1);
        sliderPannings[i].setPopupDisplayEnabled(true, false, this);
        sliderPannings[i].setLookAndFeel(&faderoniLook);
        sliderPannings[i].setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
        sliderPannings[i].textFromValueFunction = [this](const double val) { return transformPanningValueToText(val); };
        sliderPannings[i].valueFromTextFunction = [this](const String text) { return transformPanningTextToValue(text); };
        sliderPannings[i].setDoubleClickReturnValue(true, 64);
        sliderPannings[i].setValue(0);


        // this function adds the slider to the editor
        addAndMakeVisible(&sliderVolumes[i]);
        addAndMakeVisible(&sliderPannings[i]);
        addAndMakeVisible(&inputSubtrees[i]);

        // slider attachments for automation
        volumeAttachments[i].reset(new SliderAttachment(*parameters, "volume_" + String(i), sliderVolumes[i]));
        panningAttachments[i].reset(new SliderAttachment(*parameters, "panning_" + String(i), sliderPannings[i]));
    }

    setResizable(false, false);
    const auto width = 30 + min(amountOfChannels, 3) * 250;
    const auto height = std::ceil(amountOfChannels / 3) * 200;
    setSize(width, height);
}

FaderoniAudioProcessorEditor::~FaderoniAudioProcessorEditor()
{
    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        sliderVolumes[i].setLookAndFeel(nullptr);
        sliderPannings[i].setLookAndFeel(nullptr);
    }
}

//==============================================================================
void FaderoniAudioProcessorEditor::setVolume(const int& channel, const float& val)
{
    if (val == prevVolumes[channel])
        return;

    prevVolumes[channel] = val;
    sliderVolumes[channel].setValue(val, dontSendNotification);
}

void FaderoniAudioProcessorEditor::setPanning(const int& channel, const int& val)
{
    if (val == prevPannings[channel])
        return;

    prevPannings[channel] = val;
    sliderPannings[channel].setValue(val, dontSendNotification);
}

void FaderoniAudioProcessorEditor::setAmountOfChannels(const int& amount)
{
    amountOfChannels = amount;
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
    const auto width = getWidth();
    const auto height = getHeight();

    static const auto inputHeight = 20;

    lblTitle.setBounds(0, 0, width, 30);
    lblTitle.setJustificationType(Justification::Flags::centred);

    lblHost.setBounds(0, 0, 70, 30);
    lblHost.setJustificationType(Justification::Flags::centredLeft);
    inputHost.setBounds(100, 0, 100, 30);
    inputHost.setJustification(Justification::Flags::centredLeft);

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        const auto col = i % 3;
        const auto row = static_cast<int>(i / 3);
        sliderVolumes[i].setBounds(col * 250 + 50, row * 200 + 30, 80, 100);
        sliderPannings[i].setBounds(col * 250 + 150, row * 200 + 40, 60, 100);
      /*  lblSubtrees[i].setBounds();
        inputSubtrees[i].setBounds();

        sliderVolumes[i].setBounds();
        sliderPannings[i].setBounds();*/
    }
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
            volumeString = "+" + String(value, 1);
        else
            volumeString = String(value, 1);
    }

    return volumeString + " dB";
}
