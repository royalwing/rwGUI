#include "Object.h"
#include "Engine.h"

Object::Object(String inName, World* inWorld)
	: Name(inName), OuterWorld(inWorld)
{

}


void Object::Tick(float DeltaTime)
{
	TimeSinceSpawn += DeltaTime;
	if (Lifespan > 0.0f && TimeSinceSpawn > Lifespan)
	{
		delete this;
		return;
	}
	OnTick(DeltaTime);
	if (TickDelegate)
		TickDelegate(this, DeltaTime);
}

Component::Component(String inName, class Entity* inOwner)
	: Object(inName, inOwner->GetWorld()), Owner(inOwner)
{

}

Component::~Component()
{
	Owner->UnregisterComponent(this);
}


void Component::SetTickGroup(ETickGroup inTickGroup)
{
	GetWorld()->RegisterTickable(this, inTickGroup);
	TickGroup = inTickGroup;
}

void Component::RemoveFromTickGroup()
{
	GetWorld()->UnregisterTickable(this, TickGroup);
}

Transform2D WorldPresentComponent::GetWorldTransform() const
{
	return LocalTransform * GetOwner()->GetTransform();
}

Entity::Entity(String inName, World* inWorld)
	: Object(inName, inWorld)
{
	inWorld->Entities.Add(this); // Register this entity in world
}

Entity::~Entity()
{
	for (auto CompItr = GetComponents().Itr(); CompItr.IsValid(); CompItr = CompItr.Next(false)) // Setting true causing entry skip
	{
		delete CompItr.Get(); // if container has one item left after deletion inside iteration it crashes
	}
	GetWorld()->Entities.Remove(this);
}

void Entity::RegisterComponent(Component* inComponent)
{
	Components.Add(inComponent);
}

void Entity::UnregisterComponent(Component* inComponent)
{
	Components.Remove(inComponent);
}

void Entity::SetTickGroup(ETickGroup inTickGroup)
{
	GetWorld()->RegisterTickable(this, inTickGroup);
	TickGroup = inTickGroup;
}

void Entity::RemoveFromTickGroup()
{
	GetWorld()->UnregisterTickable(this, TickGroup);
}