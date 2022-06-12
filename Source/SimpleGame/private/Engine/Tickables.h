#pragma once

enum class ETickGroup : unsigned
{
    None = 0,
	PreTick,
	Tick, // Default one
	PostTick,
	Physics,
	PostPhysics,

	// To be able to iterate
	TG_MAX,
};


class ITickable
{
public:
    virtual void Tick(float DeltaTime, ETickGroup TickGroup) = 0;
    virtual void SetTickGroup(ETickGroup TickGroup) = 0;
    virtual void RemoveFromTickGroup() = 0;
};