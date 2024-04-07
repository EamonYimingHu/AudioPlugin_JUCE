/*
  ==============================================================================

    FMSynth.cpp
    Created: 2 Mar 2024 5:56:48pm
    Author:  B250029

  ==============================================================================
*/
#include "FMSynth.h"

void FMSynth::setSampleRate(float sr) {
    sampleRate = sr;
    sinOsc.setSampleRate(sr);
    triOsc.setSampleRate(sr);
    sawOsc.setSampleRate(sr);
    squareOsc.setSampleRate(sr);
    lfoTri.setSampleRate(sr);
    lfoSin.setSampleRate(sr);
    lfoSaw.setSampleRate(sr);
    lfoSaw.setFrequency(0.01f);
    lfoTri.setFrequency(0.01f);
    lfoSin.setFrequency(0.1*lfoSaw.process());
}


float FMSynth::processFMBeating(int BeatingFre, float depth) {
    fmDepth = depth;
    lfoSin.setFrequency((lfoTri.process() + 1) / 30);
    fmMod = lfoSin.process() * fmDepth;
    sinOsc.setFrequency(BeatingFre + fmMod);
    float oscOut = 0.5*sinOsc.process();
    sinOsc.setFrequency(BeatingFre + fmMod +1);
    oscOut += 0.5*sinOsc.process();
    return oscOut;
}


float FMSynth::processFMSin(int midiNoteNumber, float depth) {
    fmDepth = depth;
    lfoSin.setFrequency((lfoTri.process() + 1) / 30);
    fmMod = lfoSin.process() * fmDepth;
    sinOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    float oscOut = sinOsc.process();
    return oscOut;
}

float FMSynth::processFMTri(int midiNoteNumber, float depth) {
    srand(static_cast<unsigned int>(time(nullptr)));
    lfoSin.setFrequency((lfoTri.process() + 1) / 30);
    fmDepth = depth + lfoSin.process() * 50;
    fmMod = lfoTri.process() * fmDepth;
    float gain = lfoSin.process() + 1.5;
    triOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber + (rand() % 700 - 400) / 100) + fmMod);
    float oscOut = gain * triOsc.process();
    return oscOut;
}

float FMSynth::processFMSaw(int midiNoteNumber, float depth) {
    lfoSin.setFrequency((lfoTri.process() + 1) / 30);
    fmDepth = depth + lfoSin.process() * 20;
    fmMod = lfoTri.process() * fmDepth;
    sawOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    float oscOut = sawOsc.process();
    return oscOut;
}

float FMSynth::processFMSquare(int midiNoteNumber, float depth) {
    fmDepth = depth;
    lfoSin.setFrequency(0.01f);
    float lfoSin_Val = lfoSin.process();
    fmMod = lfoSin_Val * fmDepth;
    squareOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    squareOsc.setPulseWidth(lfoSin.process() / 2 + 0.5f);
    float oscOut = squareOsc.process();
    return oscOut;
}
