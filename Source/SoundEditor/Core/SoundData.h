#pragma once
#include <Common/Events.h>
#include <Common/Containers.h>
#include <rwgui.h>

using namespace std;

struct Timespan
{
public:
	Timespan(double inStart, double inEnd) { Start = inStart; End = inEnd; FixTimespan(); };

	double GetDuration() const { return End - Start; };
	double GetStart() const { return Start; };
	double GetEnd() const { return End; };

	void SetStart(double inStart) { Start = inStart; FixTimespan(); OnTimespanChanged.Broadcast(Start, End); };
	void SetEnd(double inEnd) { End = inEnd; FixTimespan(); OnTimespanChanged.Broadcast(Start, End); };
	void SetStartAndEnd(double inStart, double inEnd) { Start = inStart; End = inEnd; FixTimespan(); OnTimespanChanged.Broadcast(Start, End); };

	MulticastEvent<float, float> OnTimespanChanged; // float Start, float End
private:
	Timespan() : Start(0), End(0) {};
	
	void FixTimespan() { if (End < Start) End = Start; };


	double Start = 0;
	double End = 0;
};

enum ESoundModifierInfluenceType
{
	ESMIT_Add,
	ESMIT_Multiply,
	ESMIT_Subtract,
	ESMIT_Disabled
};

class SoundModifier
{
public:
	Timespan& GetTimespan() { return CurrentTimespan; };
	virtual double GetInfluenceAtTime(double inTime) const;
	virtual void ModifyFrequency(double Time, double& Frequency) const {};
	virtual void ModifyAmplitude(double Time, double& Amplitude) const {};

	
	virtual Color GetUIColor() const { return Color(0.3f, 0.3f, 0.3f, 1.0f); };

	MulticastEvent<SoundModifier*> OnModifierAdded;
	MulticastEvent<SoundModifier*> OnModifierRemoved;
	MulticastEvent<ESoundModifierInfluenceType> OnInfluenceTypeChanged;

	ESoundModifierInfluenceType GetInfluenceType() const { return InfluenceType; };
	void SetInfluenceType(ESoundModifierInfluenceType inInfluenceType);

	void AddModifier(SoundModifier* inModifier);
	void RemoveModifier(SoundModifier* inModifier);
	const List<SoundModifier*>& GetSubmodifiers() const { return SubModifiers; };
private:
	ESoundModifierInfluenceType InfluenceType;
	Timespan CurrentTimespan = Timespan(0, 1.0f);
	List<SoundModifier*> SubModifiers = { 0 };
};

class SoundTrack
{
public:
	double GetAmplitudeAtTime(double Time) const;
	double GetFrequencyAtTime(double Time) const;

	void AddModifier(SoundModifier* inModifier);
	void RemoveModifier(SoundModifier* inModifier);

	MulticastEvent<SoundModifier*> OnModifierAdded;
	MulticastEvent<SoundModifier*> OnModifierRemoved;
private:
	List<SoundModifier*> Modifiers = { 0 };
};

class SoundProject
{
public:
	MulticastEvent<SoundTrack*> OnTrackAdded;
	MulticastEvent<SoundTrack*> OnTrackRemoved;

	void AddTrack(SoundTrack* inTrack);
	void RemoveTrack(SoundTrack* inTrack);
private:
	List<SoundTrack*> Tracks;
};