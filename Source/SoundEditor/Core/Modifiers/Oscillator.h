#pragma once

#include "../SoundData.h"

enum EWaveType
{
    Sine,
    Triangle,
    Square,
    ReverseSaw,
    Saw,
    Noise
};


class Oscillator : public SoundModifier
{
public:
    virtual void ModifyAmplitude(double Time, double& Amplitude) const override;
    /*
	virtual double ModifySampleAtTime(double inSample, double inTime) const override;*/
    virtual double GetAmplitude(double Time) const { return 1.0f; };
private:
    EWaveType Type = EWaveType::Sine;
    double DefaultFrequency = 400.0f;
};