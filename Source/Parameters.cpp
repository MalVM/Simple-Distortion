#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, 
                          const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);  // parameter does not exist or wrong type
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 1) + " dB";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

static juce::String stringFromHz(float value, int)
{
    if (value < 1000.0f) {
        return juce::String(int(value)) + " Hz";
    } else if (value < 10000.0f) {
        return juce::String(value / 1000.0f, 2) + " k";
    } else {
        return juce::String(value / 1000.0f, 1) + " k";
    }
}

static float hzFromString(const juce::String& str)
{
    float value = str.getFloatValue();
    if (value < 20.0f) {
        return value * 1000.0f;
    }
    return value;
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    castParameter(apvts, distortionParamID, distortionParam);
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, mixParamID, mixParam);
    castParameter(apvts, lowCutParamID, lowCutParam);
    castParameter(apvts, highCutParamID, highCutParam);
    castParameter(apvts, bypassParamID, bypassParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        distortionParamID,
        "Distortion Gain",
        juce::NormalisableRange<float> { 0.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float> { -12.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        mixParamID,
        "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        lowCutParamID,
        "Low Cut",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
        20.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction(stringFromHz)
            .withValueFromStringFunction(hzFromString)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        highCutParamID,
        "High Cut",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f),
        20000.0f,
        juce::AudioParameterFloatAttributes()
            .withStringFromValueFunction(stringFromHz)
            .withValueFromStringFunction(hzFromString)
    ));

   

    layout.add(std::make_unique<juce::AudioParameterBool>(
        bypassParamID, "Bypass", false));

    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02;
    gainSmoother.reset(sampleRate, duration);
    distortionSmoother.reset(sampleRate, duration);
    mixSmoother.reset(sampleRate, duration);
    lowCutSmoother.reset(sampleRate, duration);
    highCutSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    gain = 0.0f;
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));

    distortion = 0.0f;
    distortionSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(distortionParam->get()));

    mix = 1.0f;
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);

    lowCut = 20.0f;
    lowCutSmoother.setCurrentAndTargetValue(lowCutParam->get());

    highCut = 20000.0f;
    highCutSmoother.setCurrentAndTargetValue(highCutParam->get());
}

void Parameters::update() noexcept
{
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));

    distortionSmoother.setTargetValue(juce::Decibels::decibelsToGain(distortionParam->get()));

    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
    lowCutSmoother.setTargetValue(lowCutParam->get());
    highCutSmoother.setTargetValue(highCutParam->get());
    bypassed = bypassParam->get();
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    distortion = distortionSmoother.getNextValue();

    mix = mixSmoother.getNextValue();

    lowCut = lowCutSmoother.getNextValue();
    highCut = highCutSmoother.getNextValue();
}
