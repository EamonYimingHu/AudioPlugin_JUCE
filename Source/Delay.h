/*
  ==============================================================================

    Delay.h

  ==============================================================================
*/

#pragma once
class Delay
{
public:
    ~Delay()
    {
        delete[] buffer;
    }

    /// write a sample into the delay line and return the delayed value
    float process(float inputSample)
    {
        float output = readVal();
        writeVal(inputSample + output * feedback);
        return output;
    }

    float readVal()
    {
        // get current value at readPos
        float outVal = linearInterpolation();
        // float outVal = buffer[readPos];
        // increment readPos
        readPos++;

        if (readPos >= size)
        {
            readPos = 0;
        }

        return outVal;
    }

    float linearInterpolation()
    {
        // get buffer index
        int indexA = floor(readPos);
        int indexB = indexA + 1;

        // wrap
        if (indexB >= size)
            indexB -= size;

        float valA = buffer[indexA];
        float valB = buffer[indexB];

        float remainder = readPos - indexA;
        float interpolatedValue = (1 - remainder) * valA + remainder * valB;

        return interpolatedValue;
    }

    void writeVal(float inputSample)
    {
        // store current value at writePos
        buffer[writePos] = inputSample;

        // increment readPos
        writePos++;
        if (writePos >= size)
        {
            writePos = 0;
        }
    }

    /// Set the size of the delay line in samples
    void setSize(int sizeInSamples)
    {
        size = sizeInSamples;
        buffer = new float[size];

        // initialize all values to zero
        for (int i = 0; i < size; i++)
        {
            buffer[i] = 0.0;
        }
    }

    /// Set the delay time in samples
    void setDelayTime(int _delayTimeInSamples)
    {
        delayTimeInSamples = _delayTimeInSamples;

        if (delayTimeInSamples > size)
        {
            delayTimeInSamples = size;
        }
        if (delayTimeInSamples < 1)
        {
            delayTimeInSamples = 1;
        }

        readPos = writePos - delayTimeInSamples;

        // if readPos < 0, then add size to readPos
        if (readPos < 0)
        {
            readPos += size;
        }
    }


    /// set feedback (0-1)
    void setFeedBack(float _fb)
    {
        feedback = _fb;
        if (feedback > 1.0)
            feedback = 1.0;

        if (feedback < 0)
            feedback = 0.0;
    }
private:
    float* buffer;  // store samples
    int size;       // size of buffer in samples

    int delayTimeInSamples;
    int readPos = 0;    // read position
    int writePos = 0;   // write position
    float feedback = 0;

};