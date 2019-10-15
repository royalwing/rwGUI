#include "Object.h"

Object::Object(String inName, World* inWorld)
{

}

void Object::Tick(float DeltaTime)
{
	OnTick(DeltaTime);
	if (TickDelegate)
		TickDelegate(this, DeltaTime);
}

Component::Component(String inName, class Entity* inOwner)
	: Object(inName, inOwner->GetWorld()), Owner(inOwner)
{

}

Transform2D VisualComponent::GetWorldTransform() const
{
	return LocalTransform * GetOwner()->GetTransform();
}

void Entity::RegisterComponent(Component* inComponent)
{
	Components.Add(inComponent);
}

void Entity::UnregisterComponent(Component* inComponent)
{
	Components.Remove(inComponent);
}
