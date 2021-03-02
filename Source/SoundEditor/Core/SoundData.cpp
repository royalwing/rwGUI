#include "SoundData.h"

double SoundModifier::GetInfluenceAtTime(double inTime) const
{
    return 1.0f;
}

void SoundModifier::SetInfluenceType(ESoundModifierInfluenceType inInfluenceType)
{
    if(InfluenceType!=inInfluenceType)
    {
        InfluenceType = inInfluenceType;
        OnInfluenceTypeChanged.Broadcast(InfluenceType);
    }
}

void SoundModifier::AddModifier(SoundModifier* inModifier)
{
    SubModifiers.Add(inModifier);
    OnModifierAdded.Broadcast(inModifier);
}

void SoundModifier::RemoveModifier(SoundModifier* inModifier)
{
    if(SubModifiers.Remove(inModifier))
    {
        OnModifierRemoved.Broadcast(inModifier);
    }
}

void SoundProject::AddTrack(SoundTrack* inTrack)
{
    Tracks.Add(inTrack);
    OnTrackAdded.Broadcast(inTrack);
}

void SoundProject::RemoveTrack(SoundTrack* inTrack)
{
    if(Tracks.Remove(inTrack))
    {
        OnTrackRemoved.Broadcast(inTrack);
    }
}

double SoundTrack::GetAmplitudeAtTime(double Time) const
{
    double Amplitude = 0.0;
    for(SoundModifier* Modifier : Modifiers)
    {
        Modifier->ModifyAmplitude(Time, Amplitude);
    }
    return Amplitude;
}

double SoundTrack::GetFrequencyAtTime(double Time) const
{
    double Frequency = 0.0;
    for (SoundModifier* Modifier : Modifiers)
    {
        Modifier->ModifyFrequency(Time, Frequency);
    }
    return Frequency;
}

void SoundTrack::AddModifier(SoundModifier* inModifier)
{
    Modifiers.Add(inModifier);
    OnModifierAdded.Broadcast(inModifier);
}

void SoundTrack::RemoveModifier(SoundModifier* inModifier)
{
    if(Modifiers.Remove(inModifier))
    {
        OnModifierRemoved.Broadcast(inModifier);
    }
}
