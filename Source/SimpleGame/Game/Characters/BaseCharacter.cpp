#include "BaseCharacter.h"
#include "Engine/Components/SpriteComponent.h"
#include "Engine/Components/PhysicsBodyComponent.h"

BaseCharacter::BaseCharacter(String Name, World* inWorld)
	: Entity(Name, inWorld)
{
	
	SpriteComponent* Sprite = new SpriteComponent("Sprite", this, "D:/player.png");
	RegisterComponent(Sprite);
	Sprite->SetSpriteSize({ 64,64 });
	Sprite->SetSortOrder(1);

	Physics = CreateComponent<PhysicsBodyComponent>("Body");
	Physics->SetSphereRadius(40.0f);

	MovementSpeed = 400000.0f;
}

BaseCharacter::~BaseCharacter()
{
}


void BaseCharacter::AddAcceleration(const Vector2D& inAcceleration)
{
	Physics->AddAcceleration(inAcceleration);
}

void BaseCharacter::OnTick(float DeltaTime)
{
	AddAcceleration(MovementInput * MovementSpeed * DeltaTime);
}
