#include "BaseCharacter.h"
#include "Engine/Components/SpriteComponent.h"

BaseCharacter::BaseCharacter(String Name, World* inWorld)
	: Entity(Name, inWorld)
{
	
	SpriteComponent* Sprite = new SpriteComponent("Sprite", this, "D:/player.png");
	RegisterComponent(Sprite);
	Sprite->SetSpriteSize({ 64,64 });
	Sprite->SetSortOrder(1);

	MovementSpeed = 400.0f;
}

BaseCharacter::~BaseCharacter()
{
}

void BaseCharacter::OnTick(float DeltaTime)
{
	SetPosition(GetPosition() + MovementInput * MovementSpeed * DeltaTime);
}
