/*
  ==============================================================================

    SynthData.cpp
    I created the vector container for FMSynth and FilterSynth. I
    created a four-dimensional array to call each instance in the vector and store the outputs of
    each instance
    Different filtering is applied to different oscillators.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SynthSound.h"

//==============================================================================


void SynthSound::prepareFilterSynth(double sampleRate)
{
    sr = sampleRate;
    
    filterSynth.push_back(FilterSynth());
    filterSynth[0].setSampleRate(sampleRate);
    filterSynth[0].setLfoFrequency(0.01f);

    filterSynth.push_back(FilterSynth());
    filterSynth[1].setSampleRate(sampleRate);
    filterSynth[1].setLfoFrequency(0.01f);

    filterSynth.push_back(FilterSynth());
    filterSynth[2].setSampleRate(sampleRate);
    filterSynth[2].setLfoFrequency(0.1f);
}

void SynthSound::prepareFMSynth(double sampleRate)
{
    sr = sampleRate; 
    lfo_fm_filter.setSampleRate(sampleRate);
    lfo_fm_filter.setFrequency(0.1f);
    lfo_fm_gain.setSampleRate(sampleRate);
    lfo_fm_gain.setFrequency(0.1f);

    fmSynth.push_back(FMSynth());
    fmSynth[0].setSampleRate(sampleRate);

    fmSynth.push_back(FMSynth());
    fmSynth[1].setSampleRate(sampleRate);

    fmSynth.push_back(FMSynth());
    fmSynth[2].setSampleRate(sampleRate);
    
    fmSynth.push_back(FMSynth());
    fmSynth[3].setSampleRate(sampleRate);
}


// Process Filter Synth for every instance in the vector
float SynthSound::processFilterSynth()
{
    float filterSynthOut[3]; 
    
    // low pass sin wave; high pass Tri; bandpass square
    filterSynthOut[0] = 0.08 * filterSynth[0].processFilteredSin(0, 55);
    filterSynthOut[1] = 0.05*  filterSynth[1].processFilteredTri(2, 50);
    filterSynthOut[2] = 0.05 * filterSynth[2].processFilteredSquare(1, 59,sr);
    
    float output = filterSynthOut[0] +filterSynthOut[1] + filterSynthOut[2];
    return output;
}


// Process FM Synth for every instance in the vector
float SynthSound::processFMSynth()
{
    float lfo_fm_Val = lfo_fm_filter.process();
    float cutoff = (lfo_fm_Val * 500 + 1000) / 6;
    float Q = (lfo_fm_Val * 150.0f + 160.0f) / 4;
    filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sr, cutoff, Q));

    // initialize an arry to store FM Synth Output
    float fmSynthOut[4];
    fmSynthOut[0] = 0.08 * fmSynth[0].processFMTri(59, 100.0f);     
    fmSynthOut[1] = 0.04 * fmSynth[1].processFMSin(60 * fmSynth[0].processFMTri(59, 100.0f) , 100.0f);
    fmSynthOut[2] = 0.04 * filter.processSingleSampleRaw(fmSynth[2].processFMSaw(55, 100.0f));
    // Set the fundamental frequency of the beating effect here!
    fmSynthOut[3] = 0.08 * fmSynth[3].processFMBeating(300, 100.0f);


    float output = fmSynthOut[0] + fmSynthOut[1] + fmSynthOut[2] + fmSynthOut[3];
    return output;
}
