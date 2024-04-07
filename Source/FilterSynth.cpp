/*
  ==============================================================================

    FilterSynth.cpp
    Created: 2 Mar 2024 5:56:21pm
    Author:  B250029

  ==============================================================================
*/

#include "FilterSynth.h"

void FilterSynth::setSampleRate(float sr) {
    sampleRate = sr;
    sinOsc.setSampleRate(sr);
    triOsc.setSampleRate(sr);
    sawOsc.setSampleRate(sr);
    squareOsc.setSampleRate(sr);
    lfo_cutoff.setSampleRate(sr);
    lfo_Q.setSampleRate(sr);
}

void FilterSynth::setLfoFrequency(float f) {
    lfo_cutoff.setFrequency(f);
    lfo_Q.setFrequency(f);
}

//==============================================================================
// Coding Filter Method for each oscillator
float FilterSynth::processFilteredSin(int filterchoice, int midiNoteNumber) {
    sinOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    float OscOut = sinOsc.process();
    filterSetup(filterchoice);
    return filter.processSingleSampleRaw(OscOut);
}

float FilterSynth::processFilteredTri(int filterchoice, int midiNoteNumber) {
    triOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    float OscOut = triOsc.process();
    filterSetup(filterchoice);
    return filter.processSingleSampleRaw(OscOut);
}

float FilterSynth::processFilteredSaw(int filterchoice, int midiNoteNumber) {
    sawOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    float OscOut = sawOsc.process();
    filterSetup(filterchoice);
    return filter.processSingleSampleRaw(OscOut);
}

float FilterSynth::processFilteredSquare(int filterchoice, int midiNoteNumber, float sr) {
    squareOsc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    float OscOut = squareOsc.process();
    filterSetup(filterchoice);
    return filter.processSingleSampleRaw(OscOut);
}


// Setting up filters
// filter choice : 0 -- low pass
//                 1 -- high pass
//                 2 -- band pass
void FilterSynth::filterSetup(int filterchoice) {
    float lfo_cutoff_Val = lfo_cutoff.process();
    float lfo_Q_Val = lfo_Q.process();
    float cutoff, Q;

    switch (filterchoice) {
    case 0:
        cutoff = (lfo_cutoff_Val * 900 + 1100)/6 ;
        Q = (lfo_Q_Val * 150.0f + 170.0f)/4 ;
        filter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoff, Q));
        break;
    case 1:
        cutoff = (lfo_cutoff_Val * 500 + 600) / 6;
        Q = (lfo_Q_Val * 500.0f + 510.0f) / 4;
        filter.setCoefficients(juce::IIRCoefficients::makeHighPass(sampleRate, cutoff, Q));
        break;
    case 2:
        cutoff = (lfo_cutoff_Val * 2000 + 2100) / 3;
        Q = (lfo_Q_Val * 500.0f + 510.0f) / 4;
        filter.setCoefficients(juce::IIRCoefficients::makeBandPass(sampleRate, cutoff, Q));
        break;
    default:
        jassertfalse;   // Error warning
        break;
    }
}
