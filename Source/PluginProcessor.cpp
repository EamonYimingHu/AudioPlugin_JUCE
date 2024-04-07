/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DroneMusicAudioProcessor::DroneMusicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

DroneMusicAudioProcessor::~DroneMusicAudioProcessor()
{
}

//==============================================================================
const juce::String DroneMusicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DroneMusicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DroneMusicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DroneMusicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DroneMusicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DroneMusicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DroneMusicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DroneMusicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DroneMusicAudioProcessor::getProgramName (int index)
{
    return {};
}

void DroneMusicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DroneMusicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    squareOsc.setSampleRate(sampleRate);
    squareOsc.setFrequency(200.0f);

    envControlFS.setSampleRate(sampleRate);
    envControlFS.setFrequency(0.1f);

    envControlFM.setSampleRate(sampleRate);
    envControlFM.setFrequency(0.004f);

    envControlbass.setSampleRate(sampleRate);
    envControlbass.setFrequency(0.008f);       //env fre=0.008=1/120 Since I'd like the period = 120s

    lfo.setSampleRate(sampleRate);
    lfo.setFrequency(0.05f);


    data.prepareFilterSynth(sampleRate);
    data.prepareFMSynth(sampleRate);

    bass.preparebass(sampleRate);

    sr = sampleRate;

    filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 4000.0f, 5.0f));
    filter.reset();

    juce::Reverb::Parameters reverbParams;
    reverbParams.dryLevel = 0.5f;
    reverbParams.wetLevel = 0.5f;
    reverbParams.roomSize = 0.99f;

    reverb.setParameters(reverbParams);
    reverb.reset();


    delay.setSize(sampleRate * 2);
    delay.setDelayTime(0.25 * sampleRate);

    panningLfo.setSampleRate(sampleRate);
    panningLfo.setFrequency(0.25);
}

void DroneMusicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DroneMusicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DroneMusicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

 


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    int numSamples = buffer.getNumSamples();

    // pointers to audio arrays
    auto* leftChannel = buffer.getWritePointer(0); //int channelNumber
    auto* rightChannel = buffer.getWritePointer(1);
    
    
    // DSP Loop!

    for (int i = 0; i < numSamples; i++) {
        // Setup Panning
        float panVal = panningLfo.process();
        float leftGain = (panVal * 0.5f) + 0.5f;  // 0 - 1
        float rightGain = 1.0f - leftGain;         // 1 - 0
        
        // Setup Delay
        float delayMod = panningLfo.process();
        float delayTime = delayMod * 200 + 5000;
        delay.setDelayTime(delayTime);
        
        // Making Envolope for FMsynth&FilterSynth&bass here 
        float env_fm = envControlFM.process();
        float env_fs = envControlFS.process();
        float env_bass = 1.6*envControlbass.process();


        //Control the envolope for different segment    
        if (env_fm > 0)                     
        { 
            env_fm *= 0.8f;
            env_fs *= 0.1f;
        }
        
        else                                
        { 
            env_fm *= 0.6f;
            env_fs *= 0.1f;
        }

        // Set when the drum/bass sound starts(here it starts at 120s)
        if (env_bass > 0)
        {
            env_bass = 0;
        }




        //// process FM Synth and delay
        float sample_fm = data.processFMSynth();
        float delayedSample = 0.3 * delay.process(sample_fm);

        // process filter Synth
        float sample_fs = data.processFilterSynth();

        // process bass sound
        float sample_bass = bass.makebass1() + bass.makebass2();


        leftChannel[i] = ((delayedSample + sample_fm) * env_fm + sample_fs * env_fs +  sample_bass *env_bass) * leftGain * 0.5f;
        rightChannel[i] = ((delayedSample + sample_fm) * env_fm + sample_fs * env_fs +  sample_bass *env_bass) * rightGain * 0.5f;
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    reverb.processStereo(leftChannel, rightChannel, numSamples);

}

//==============================================================================
bool DroneMusicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DroneMusicAudioProcessor::createEditor()
{
    return new DroneMusicAudioProcessorEditor (*this);
}

//==============================================================================
void DroneMusicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DroneMusicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DroneMusicAudioProcessor();
}
