/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterSynth.h"
#include "FMSynth.h"
#include "Delay.h"
#include "SynthSound.h"
#include "Bass.h"

//==============================================================================
/**
*/
class DroneMusicAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DroneMusicAudioProcessor();
    ~DroneMusicAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DroneMusicAudioProcessor)

    float gain = 0.1f;
    float sr; // sample rate
    juce::Random random;
    juce::Reverb reverb;
    juce::IIRFilter filter;

    SquareOsc squareOsc;
    SinOsc envControlFM, envControlFS, envControlbass;

    //SinOsc gainVolFm;

    SinOsc lfo;

    FilterSynth filterSynth;
    FMSynth fmSynth;

    Delay delay;        // instance of our delay class
    SinOsc panningLfo;

    SynthSound data;
    Bass bass;
    float env_bass = 0;
};
