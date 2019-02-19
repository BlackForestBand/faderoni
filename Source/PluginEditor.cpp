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
    faderoniLook.setDefaultSansSerifTypeface(bodyFont.getTypeface());


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    lblTitle.setFont(headerFont);
    lblTitle.setText("Faderoni", dontSendNotification);

    lblHost.setFont(bodyFont);
    lblHost.setText("API-Host:", dontSendNotification);

    inputHost.onTextChange = [this]() { processor.setHost(inputHost.getText()); };
    inputHost.setText(parameters->getParameterAsValue("hostname").getValue(), false);

    lblAmountOfChannels.setText(parameters->getParameterAsValue("amount_of_channels").getValue(), dontSendNotification);
    lblAmountOfChannels.setFont(headerFont);
    btnMinus.setLookAndFeel(&faderoniLook);
    btnMinus.setButtonText("-");
    btnMinus.onClick = [this]()
    {
        setAmountOfChannels(amountOfChannels > 1 ? amountOfChannels - 1 : 1);
        processor.setAmountOfChannels(amountOfChannels);
    };
    btnPlus.setLookAndFeel(&faderoniLook);
    btnPlus.setButtonText("+");
    btnPlus.onClick = [this]()
    {
        setAmountOfChannels(amountOfChannels = amountOfChannels < 6 ? amountOfChannels + 1 : 6);
        processor.setAmountOfChannels(amountOfChannels);
    };

    lblMasterVolume.setFont(bodyFont);
    lblMasterVolume.setText("Master Volume:", dontSendNotification);

    sliderMasterVolume.setSliderStyle(Slider::LinearHorizontal);
    sliderMasterVolume.setRange(-48, 12, 0.1);
    sliderMasterVolume.setPopupDisplayEnabled(true, false, this);
    sliderMasterVolume.setLookAndFeel(&faderoniLook);
    sliderMasterVolume.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, false, 80, 20);
    sliderMasterVolume.setDoubleClickReturnValue(true, 102);
    sliderMasterVolume.setValue(0);



    addAndMakeVisible(&sliderMasterVolume);
    addAndMakeVisible(&lblMasterVolume);
    addAndMakeVisible(&lblTitle);
    addAndMakeVisible(&lblHost);
    addAndMakeVisible(&inputHost);
    addAndMakeVisible(&lblAmountOfChannels);
    addAndMakeVisible(&btnMinus);
    addAndMakeVisible(&btnPlus);


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
        sliderVolumes[i].setDoubleClickReturnValue(true, 102);
        sliderVolumes[i].setValue(0);

        // these define the parameters of our slider object
        sliderPannings[i].setSliderStyle(Slider::RotaryHorizontalDrag);
        sliderPannings[i].setRange(-100, 100, 1);
        sliderPannings[i].setPopupDisplayEnabled(true, false, this);
        sliderPannings[i].setLookAndFeel(&faderoniLook);
        sliderPannings[i].setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 60, 20);
        sliderPannings[i].setDoubleClickReturnValue(true, 64);
        sliderPannings[i].setValue(0);


        // this function adds the slider to the editor
        addAndMakeVisible(&sliderVolumes[i]);
        addAndMakeVisible(&sliderPannings[i]);
        addAndMakeVisible(&inputSubtrees[i]);

        // slider attachments for automation
        volumeAttachments[i].reset(new SliderAttachment(*parameters, "volume_" + String(i), sliderVolumes[i]));
        panningAttachments[i].reset(new SliderAttachment(*parameters, "panning_" + String(i), sliderPannings[i]));
        masterVolumeAttachment.reset(new SliderAttachment(*parameters, "master_volume", sliderMasterVolume));
    }

    setResizable(false, false);
    setSize();
}

FaderoniAudioProcessorEditor::~FaderoniAudioProcessorEditor()
{
    btnMinus.setLookAndFeel(nullptr);
    btnPlus.setLookAndFeel(nullptr);

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        sliderVolumes[i].setLookAndFeel(nullptr);
        sliderPannings[i].setLookAndFeel(nullptr);
    }

    sliderMasterVolume.setLookAndFeel(nullptr);
}

//==============================================================================
void FaderoniAudioProcessorEditor::setVolume(const int& channel, const float& val)
{
    if (val == prevVolumes[channel])
        return;

    prevVolumes[channel] = val;
    sliderVolumes[channel].setValue(val, dontSendNotification);
}

void FaderoniAudioProcessorEditor::setMasterVolume(const float& val)
{
    if (val == prevMasterVolume)
        return;

    prevMasterVolume = val;
    sliderMasterVolume.setValue(val, dontSendNotification);
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
    lblAmountOfChannels.setText(String(amount), dontSendNotification);

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        sliderVolumes[i].setVisible(i < amountOfChannels);
        sliderPannings[i].setVisible(i < amountOfChannels);
        inputSubtrees[i].setVisible(i < amountOfChannels);
        inputSubtrees[i].setVisible(i < amountOfChannels);
    }

    setSize();
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

    static const auto inputHeight = 22;

    lblTitle.setBounds(0, 0, width, 40);
    lblTitle.setJustificationType(Justification::Flags::centred);

    lblHost.setBounds(5, 0, 75, 40);
    lblHost.setJustificationType(Justification::Flags::centredLeft);
    inputHost.setBounds(80, 9, 100, inputHeight);
    inputHost.setJustification(Justification::Flags::centred);

    btnMinus.setBounds(width - 110, 7, 30, 30);
    lblAmountOfChannels.setBounds(width - 80, 7, 40, 30);
    lblAmountOfChannels.setJustificationType(Justification::Flags::centred);
    btnPlus.setBounds(width - 40, 7, 30, 30);


    lblMasterVolume.setBounds(5, height - 35, 100, 30);
    lblMasterVolume.setJustificationType(Justification::Flags::centred);

    sliderMasterVolume.setBounds(105, height - 30, 300, 20);

    for (auto i = 0; i < FADERONI_MAX_CHANNELS; i++) {
        const auto col = i % 3;
        const auto row = i / 3;

        sliderVolumes[i].setBounds(col * 250 + 50, row * 220 + 40, 80, 120);
        sliderPannings[i].setBounds(col * 250 + 150, row * 220 + 80, 60, 80);
        inputSubtrees[i].setBounds(col * 250 + 50, row * 220 + 180, 160, inputHeight);
        inputSubtrees[i].setJustification(Justification::Flags::centred);
    }
}

void FaderoniAudioProcessorEditor::setSize()
{
    const auto width = 30 + max(min(amountOfChannels, 3), 2) * 250;
    const auto height = 50 + std::ceil(amountOfChannels / 3.0f) * 220;
    static_cast<Component*>(this)->setSize(width, height);
}
