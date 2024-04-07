#pragma once

#include "Oscillators.h"
#include <JuceHeader.h>

class FMSynth {
public:
    void setSampleRate(float sr);
    float processFMSin(int midiNoteNumber, float depth);
    float processFMTri(int midiNoteNumber, float depth);
    float processFMSaw(int midiNoteNumber, float depth);
    float processFMSquare(int midiNoteNumber, float depth);
    float processFMBeating(int BeatingFre, float depth);
private:
    SinOsc sinOsc, lfoSin;
    TriOsc triOsc, lfoTri;
    SawOsc sawOsc, lfoSaw;
    SquareOsc squareOsc;
    float fmMod{ 0.0f }, fmDepth{ 0.0f }, sampleRate;
    int flag = 0;
};
