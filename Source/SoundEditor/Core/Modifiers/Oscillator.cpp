#include "Oscillator.h"
#define _USE_MATH_DEFINES
#include <math.h>


void Oscillator::ModifyAmplitude(double Time, double& Amplitude) const
{
    double CurrentFrequency = DefaultFrequency;
    double CurrentAmplitude = 0.0f;
    for(auto Modifier : GetSubmodifiers())
    {
        Modifier->ModifyFrequency(Time, CurrentFrequency);
        Modifier->ModifyAmplitude(Time, CurrentAmplitude);
    }


    auto Oscilate = [CurrentFrequency, CurrentAmplitude, Time, this]()->double 
    {
        if (Time <= 0.0f)
        {
            return 0.0f;
        };

        switch (Type)
        {
        case EWaveType::Sine:
            return sin(Time * M_PI * 2 * CurrentFrequency) * CurrentAmplitude;
        case EWaveType::Square:
            return round(sin(Time * M_PI * 2 * CurrentFrequency)) * CurrentAmplitude;
        case EWaveType::Saw:
            return ((fmod(Time * CurrentFrequency, 1.0) * 2.0) - 1.0) * CurrentAmplitude;
        case EWaveType::ReverseSaw:
            return 1.0 - ((fmod(Time * CurrentFrequency, 1.0) * 2.0) - 1.0) * CurrentAmplitude;
        case EWaveType::Triangle:
            return 0.0;
            //return FMath::GetMappedRangeValueUnclamped(FVector2D(0.0f, 0.5f), FVector2D(-1.0f, 1.0f), abs(0.5f - fmod(inTime * Freq, 1.0f))) * Amp;
        default:
            break;
        }
        return 0.0;
    };

    double FinalValue = Oscilate();
    switch(GetInfluenceType())
    {
    case ESoundModifierInfluenceType::ESMIT_Add:
        Amplitude += FinalValue * GetInfluenceAtTime(Time);
        break;
    case ESoundModifierInfluenceType::ESMIT_Multiply:
        Amplitude = rwMath::lerp(Amplitude, Amplitude * FinalValue, GetInfluenceAtTime(Time));
        break;
    case ESoundModifierInfluenceType::ESMIT_Subtract:
        Amplitude -= FinalValue * GetInfluenceAtTime(Time);
    default:
        break;
    }
}
