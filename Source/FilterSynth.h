#pragma once

#include "Oscillators.h"
#include <JuceHeader.h>

class FilterSynth {
public:
    void setSampleRate(float sr);
    void setLfoFrequency(float f);
    float processFilteredSin(int filterchoice, int midiNoteNumber);
    float processFilteredTri(int filterchoice, int midiNoteNumber);
    float processFilteredSaw(int filterchoice, int midiNoteNumber);
    float processFilteredSquare(int filterchoice, int midiNoteNumber, float sr);
    void filterSetup(int filterchoice);

private:
    SinOsc sinOsc, triOsc, sawOsc, squareOsc, lfo_cutoff, lfo_Q;
    juce::IIRFilter filter;
    float sampleRate;
    float freq{ 0.0f };
};
