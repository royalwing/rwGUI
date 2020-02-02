#pragma once

#include "../Object.h"

enum EPhysicsBodyType
{
	PBT_Circle,
	PBT_Box,
	PBT_Capsule,
	PBT_Rectangle
};

namespace rw
{
	namespace physics
	{
		static void SphereSphereIntersect(Vector2D p1, float r1, Vector2D p2, float r2, float& Distance)
		{
			const float DistanceBetweenCenters = (p2 - p1).Length();
			Distance = r1 + r2 - DistanceBetweenCenters;
		}
	}

}

class PhysicsBodyComponent : public Component
{
	using Component::Component;
private:
	Vector2D LinearAcceleration;
	float AngularAcceleration;
	Vector2D LinearVelocity;
	float AngularVelocity;
	float Radius;
public:


	float MaxAcceleration = 3000.0f;
	float Deceleration = 150000.0f;
	float VelocityLoss = 2100.0f;

	PhysicsBodyComponent(String inName, Entity* inOwner, EPhysicsBodyType inType);

	void SetSphereRadius(const float& inRadius);

	void AddAcceleration(const Vector2D& Acceleration);
	void SetVelocity(const Vector2D& newVelocity);

	virtual void OnTick(float DeltaTime) override;
};