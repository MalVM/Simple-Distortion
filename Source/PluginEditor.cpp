/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DistortionAudioProcessorEditor::DistortionAudioProcessorEditor (DistortionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mixGainGroup.addAndMakeVisible(mixKnob);
    mixGainGroup.addAndMakeVisible(outputGainKnob);
    addAndMakeVisible(mixGainGroup);

    filterGroup.addAndMakeVisible(highcutKnob);
    filterGroup.addAndMakeVisible(lowcutKnob);
    addAndMakeVisible(filterGroup);

    driveGroup.addAndMakeVisible(disGainKnob);
    addAndMakeVisible(driveGroup);

    addAndMakeVisible(bypassButton);


    setSize (500, 330);
    setLookAndFeel(&mainLF);
}

DistortionAudioProcessorEditor::~DistortionAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void DistortionAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkblue);
}

void DistortionAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();

    int y = 50;
    int height = bounds.getHeight() - 60;

    driveGroup.setBounds(10, y, 110, height);

    mixGainGroup.setBounds(bounds.getWidth() - 160, y, 150, height);

    filterGroup.setBounds(driveGroup.getRight() + 10, y, mixGainGroup.getX() - driveGroup.getRight() - 20, height);

    disGainKnob.setTopLeftPosition(20, 20);

    mixKnob.setTopLeftPosition(20, 20);
    outputGainKnob.setTopLeftPosition(20, mixKnob.getBottom() + 20);
    bypassButton.setTopLeftPosition(20, outputGainKnob.getBottom() + 20);

    lowcutKnob.setTopLeftPosition(20, 20);
    highcutKnob.setTopLeftPosition(lowcutKnob.getRight()+ 20, 20);
}
