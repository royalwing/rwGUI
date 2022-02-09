#pragma once

#include "Engine/Object.h"

class BaseCharacter : public Entity
{
private:
	Vector2D MovementInput;
	class PhysicsBodyComponent* Physics;
public:
	BaseCharacter(String Name, World* inWorld);
	~BaseCharacter();

	float MovementSpeed;

	void SetMovementInput(Vector2D Input) { MovementInput = Input; };
	Vector2D GetVelocity() const { return MovementInput * MovementSpeed; };

	void AddAcceleration(const Vector2D& inAcceleration);

	virtual void OnTick(float DeltaTime) override;
};