#pragma once

#include <JuceHeader.h>

const juce::ParameterID gainParamID { "gain", 1 };
const juce::ParameterID mixParamID { "mix", 1 };
const juce::ParameterID lowCutParamID { "lowCut", 1 };
const juce::ParameterID highCutParamID { "highCut", 1 };
const juce::ParameterID bypassParamID { "bypass", 1 };
const juce::ParameterID distortionParamID{ "distortion", 1 };

class Parameters
{
public:
    Parameters(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    void prepareToPlay(double sampleRate) noexcept;
    void reset() noexcept;
    void update() noexcept;
    void smoothen() noexcept;

    float gain = 0.0f;
    float distortion = 0.0f;
    float mix = 1.0f;
    float lowCut = 20.0f;
    float highCut = 20000.0f;
    bool bypassed = false;

    juce::AudioParameterBool* bypassParam;

private:
    juce::AudioParameterFloat* gainParam;
    juce::LinearSmoothedValue<float> gainSmoother;

    juce::AudioParameterFloat* distortionParam;
    juce::LinearSmoothedValue<float> distortionSmoother;

    juce::AudioParameterFloat* mixParam;
    juce::LinearSmoothedValue<float> mixSmoother;

    juce::AudioParameterFloat* lowCutParam;
    juce::LinearSmoothedValue<float> lowCutSmoother;

    juce::AudioParameterFloat* highCutParam;
    juce::LinearSmoothedValue<float> highCutSmoother;
};
