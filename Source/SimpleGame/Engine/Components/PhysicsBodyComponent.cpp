#include "PhysicsBodyComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent(String inName, Entity* inOwner, EPhysicsBodyType inType)
	: Component(inName, inOwner)
{
	LinearAcceleration = 0.0f;
	LinearVelocity = 0.0f;
}

void PhysicsBodyComponent::SetSphereRadius(const float& inRadius)
{
	Radius = inRadius;
}

void PhysicsBodyComponent::AddAcceleration(const Vector2D& Acceleration)
{
	LinearAcceleration += Acceleration;
	if (LinearAcceleration.Length() > MaxAcceleration) LinearAcceleration = LinearAcceleration.GetNormalized() * MaxAcceleration;
}

void PhysicsBodyComponent::OnTick(float DeltaTime)
{
	LinearVelocity += LinearAcceleration * DeltaTime;
	LinearVelocity -= LinearVelocity.GetNormalized()*min(VelocityLoss*DeltaTime, LinearVelocity.Length());
	GetOwner()->SetPosition(GetOwner()->GetPosition() + LinearVelocity * DeltaTime);
	LinearAcceleration -= LinearAcceleration.GetNormalized()*min(Deceleration*DeltaTime, LinearAcceleration.Length());
}
