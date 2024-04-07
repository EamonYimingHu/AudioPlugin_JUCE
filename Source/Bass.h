/*
  ==============================================================================

    Bass.h

  ==============================================================================
*/

#pragma once
#ifndef Bass_h
#define Bass_h

#include "Oscillators.h"

//I used a square wave as an envelope and used a bass sine oscillator to 
//generate bass and sub - bass at the C3 and C2 pitches

class Bass
{
public:
    void preparebass(double sampleRate)
    {
        sqforBass1.setSampleRate(sampleRate);
        sqforBass1.setFrequency(0.5f);
        sqforBass1.setPulseWidth(0.02f);

        sqforBass2.setSampleRate(sampleRate);
        sqforBass2.setFrequency(0.25f);
        sqforBass2.setPulseWidth(0.02f);


        LowBass1.setSampleRate(sampleRate);
        LowBass1.setFrequency(65);          //65HZ = C3

        LowBass2.setSampleRate(sampleRate);
        LowBass2.setFrequency(32);          //32HZ = C2

        LPfilter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, 100));
    }


    // process the first bass sound
    float makebass1()
    {
        bassout1 = (sqforBass1.process()+0.5) * LowBass1.process();
        
        return LPfilter.processSingleSampleRaw(bassout1);
    }


    // process the sound bass sound
    float makebass2()
    {
        bassout2 = (sqforBass2.process() + 0.5) * LowBass2.process();
        return LPfilter.processSingleSampleRaw(bassout2);
    }
private:
    SquareOsc sqforBass1, sqforBass2;
    SinOsc LowBass1, LowBass2;
    float bassout1, bassout2;
    juce::IIRFilter LPfilter;
};
#endif // Bass_h