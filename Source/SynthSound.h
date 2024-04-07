/*
  ==============================================================================

    SynthSound.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "FilterSynth.h"
#include "FMSynth.h"

//==============================================================================
/*
*/
class SynthSound
{
public:
    void prepareFilterSynth(double sampleRate);
    void prepareFMSynth(double sampleRate);
    float processFilterSynth();
    float processFMSynth();

private:
    std::vector<FilterSynth> filterSynth;
    std::vector<FMSynth> fmSynth;
    juce::IIRFilter filter;
    double sr;
    SinOsc lfo_fm_filter;
    SinOsc lfo_fm_gain;

};
