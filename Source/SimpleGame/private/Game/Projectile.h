#pragma once
#include "Engine/Object.h"

class Projectile : public Entity
{
	using Entity::Entity;
private:
	class PhysicsBodyComponent* Body;
public:
	Projectile(String inName, World* inWorld);

	Vector2D Direction;

	virtual void OnTick(float DeltaTime) override;
};
