/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Parameters.h"
#include "RotaryKnob.h"
#include "LookAndFeel.h"

//==============================================================================
/**
*/
class DistortionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DistortionAudioProcessorEditor (DistortionAudioProcessor&);
    ~DistortionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RotaryKnob outputGainKnob{ "Gain", audioProcessor.apvts, gainParamID, true };
    RotaryKnob mixKnob{ "Mix", audioProcessor.apvts, mixParamID };
    RotaryKnob disGainKnob{ "Distortion", audioProcessor.apvts, distortionParamID};
    RotaryKnob highcutKnob{ "High Cut", audioProcessor.apvts, highCutParamID};
    RotaryKnob lowcutKnob{ "Low Cut", audioProcessor.apvts, lowCutParamID};
    
    juce::ImageButton bypassButton;

    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment{
    audioProcessor.apvts, bypassParamID.getParamID(), bypassButton
    };

    juce::GroupComponent driveGroup, filterGroup, mixGainGroup;

    RotaryKnobLookAndFeel mainLF;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DistortionAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistortionAudioProcessorEditor)
};
