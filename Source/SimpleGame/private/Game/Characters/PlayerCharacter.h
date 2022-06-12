#pragma once

#include "BaseCharacter.h"

class PlayerCharacter : public BaseCharacter
{
	using BaseCharacter::BaseCharacter;
	float FireRate = 20.0f;
	float TimeSinceLastFire = 0.0f;
	bool bWantsToShoot = false;
public:
	Vector2D GetAimingLocation() const;

	virtual void Tick(float DeltaTime, ETickGroup TickGroup) override;

	void StartShooting() { bWantsToShoot = true; };
	void StopShooting() { bWantsToShoot = false; };

	void Shoot();
};