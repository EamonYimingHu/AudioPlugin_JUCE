/*
  ==============================================================================

    Oscillators.h


  ==============================================================================
*/

#pragma once
#ifndef Oscillators_h
#define Oscillators_h

#include <cmath>
#include <JuceHeader.h>
// PARENT phasor class
class Phasor {
public:
    // Our parent oscillator class does the key things required for most oscillators:
    // -- handles phase
    // -- handles setters and getters for frequency and samplerate

    // update the phase and output the next sample from the oscillator
    float process() {
        phase += phaseDelta;

        if (phase > 1.0f)
            phase -= 1.0f;

        return output(phase);
    }

    virtual float output(float p) {
        return p;
    }

    void setSampleRate(float SR)
    {
        sampleRate = SR;
    }

    void setFrequency(float freq) 
    {
        frequency = freq;
        phaseDelta = frequency / sampleRate;
    }

    void setPhase(float ph)
    {
        phase = ph;
    }

private:
    float frequency;
    float sampleRate;
    float phase = 0.0f;
    float phaseDelta;
};

//==================================================

//   CHILD Class
class TriOsc : public Phasor 
{
    float output(float p) override 
    {
        return fabsf(p - 0.5f) - 0.5f;
    }
};

//   CHILD Class
//==================================================
class SinOsc : public Phasor 
{
    float output(float p) override 
    {
        return std::sin(p * 2.0 * 3.14159);
    }
};

//   CHILD Class
//==================================================
class SawOsc : public Phasor 
{
    float output(float p) override
    {
        return p / juce::MathConstants<float>::pi;
    }
};

//   CHILD Class
//==================================================
class SquareOsc : public Phasor 
{
public:
    float output(float p) override 
    {
        float outVal = 0.5;
        if (p > pulseWidth)
            outVal = -0.5;
        return outVal;
    }
    void setPulseWidth(float pw) 
    {
        pulseWidth = pw;
    }
    float pulseWidth = 0.5f;
private:
};
#endif // Oscillators_h