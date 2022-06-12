#pragma once

#include "../Object.h"

enum EPhysicsBodyType
{
	PBT_Circle,
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

class PhysicsBodyComponent : public WorldPresentComponent
{
	using WorldPresentComponent::WorldPresentComponent;
private:

	EPhysicsBodyType PhysBodyType;
	float Radius;
	Vector2D BoxSize;
	Vector2D LinearAcceleration;
	float AngularAcceleration;
	Vector2D LinearVelocity;
	float AngularVelocity;

	struct AppliedForce
	{
		Vector2D WorldPosition;
		Vector2D WorldDirection;
		Vector2D LocalPosition;
		Vector2D LocalDirection;
	};

	List<AppliedForce> AppliedForces;

	void ApplyForce(Vector2D inWorldPosition, Vector2D inWorldDirection);
public:


	float MaxAcceleration = 3000.0f;
	float Deceleration = 150000.0f;
	float VelocityLoss = 2100.0f;

	PhysicsBodyComponent(String inName, Entity* inOwner);
	~PhysicsBodyComponent();

	EPhysicsBodyType GetBodyType() const { return PhysBodyType; };

	void SetSphereRadius(const float& inRadius);
	void SetBox(const Vector2D& Size);
	Vector2D GetBoxSize() const { return BoxSize; };

	void AddAcceleration(const Vector2D& Acceleration);
	void SetVelocity(const Vector2D& newVelocity);

	virtual void Tick(float DeltaTime, ETickGroup TickGroup) override;
};