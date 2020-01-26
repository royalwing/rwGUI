#pragma once

#include "Engine/Object.h"

class BaseCharacter : public Entity
{
private:
	Vector2D MovementInput;
public:
	BaseCharacter(String Name, World* inWorld);
	~BaseCharacter();

	float MovementSpeed;

	void SetMovementInput(Vector2D Input) { MovementInput = Input; };
	Vector2D GetVelocity() const { return MovementInput * MovementSpeed; };

	virtual void OnTick(float DeltaTime) override;
};