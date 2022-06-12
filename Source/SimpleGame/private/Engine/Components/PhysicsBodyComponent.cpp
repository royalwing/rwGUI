#include "PhysicsBodyComponent.h"

PhysicsBodyComponent::PhysicsBodyComponent(String inName, Entity* inOwner)
	: WorldPresentComponent(inName, inOwner)
{
	SetTickGroup(ETickGroup::PostPhysics);

	LinearAcceleration = 0.0f;
	LinearVelocity = 0.0f;
}

PhysicsBodyComponent::~PhysicsBodyComponent()
{

}

void PhysicsBodyComponent::SetSphereRadius(const float& inRadius)
{
	PhysBodyType = EPhysicsBodyType::PBT_Circle;
	Radius = inRadius;
}

void PhysicsBodyComponent::SetBox(const Vector2D& Size)
{
	PhysBodyType = EPhysicsBodyType::PBT_Rectangle;
	BoxSize = Size;
}

void PhysicsBodyComponent::ApplyForce(Vector2D inWorldPosition, Vector2D inWorldDirection)
{
	AppliedForce Force;
	Force.WorldDirection = inWorldDirection;
	Force.WorldPosition = inWorldPosition;
}

void PhysicsBodyComponent::AddAcceleration(const Vector2D& Acceleration)
{
	LinearAcceleration += Acceleration;
	if (LinearAcceleration.Length() > MaxAcceleration) LinearAcceleration = LinearAcceleration.GetNormalized() * MaxAcceleration;
}

void PhysicsBodyComponent::SetVelocity(const Vector2D& newVelocity)
{
	LinearVelocity = newVelocity;
}

void PhysicsBodyComponent::Tick(float DeltaTime, ETickGroup TickGroup)
{
	if(TickGroup==ETickGroup::Physics)
	{
		// TODO@royalwing : We should solve all the force applications for this physics body and calculate final linear and angular velocity for this body

		return;
	}


	LinearVelocity += LinearAcceleration * DeltaTime;
	GetOwner()->SetPosition(GetOwner()->GetPosition() + LinearVelocity * DeltaTime);
	LinearVelocity -= LinearVelocity.GetNormalized()*min(VelocityLoss*DeltaTime, LinearVelocity.Length());
	LinearAcceleration -= LinearAcceleration.GetNormalized()*min(Deceleration*DeltaTime, LinearAcceleration.Length());
}
