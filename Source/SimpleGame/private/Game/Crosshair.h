#pragma once

#include "Engine/Object.h"
#include "Engine/Components/DynamicSpriteComponent.h"

class CrosshairComponent : public DynamicSpriteComponent
{
	using DynamicSpriteComponent::DynamicSpriteComponent;
public:
	virtual void OnDynamicDraw(ID2D1BitmapRenderTarget* renderTarget) override;
};

class Crosshair : public Entity
{
	using Entity::Entity;
private:
	class PlayerCharacter* OwnerCharacter;
public:
	Crosshair(String inName,World* inWorld);

	void OnTick(float DeltaTime) override;

	void SetOwningCharacter(class PlayerCharacter* inCharacter);
	class PlayerCharacter* GetOwnerCharacter() const;
};