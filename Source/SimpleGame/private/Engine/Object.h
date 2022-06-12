#pragma once
#include "Common/String.h"
#include "Engine/Tickables.h"
#include "Common/rwmath.h"

class Object
{
private:
	String Name;
	class World* OuterWorld = nullptr;
	float Lifespan = 0.0f;
	float TimeSinceSpawn = 0.0f;
	Object() = delete;
public:
	Object(String inName, class World* inWorld);
	virtual ~Object() {};

	typedef void(*OnTickDelegate)(Object* Object, float DeltaTime);
	OnTickDelegate TickDelegate = nullptr;

	virtual void OnTick(float DeltaTime) {};

	class World* GetWorld() const { return OuterWorld; };
	void SetLifespan(float inLifespan) { Lifespan = TimeSinceSpawn + inLifespan; };
	String GetName() const { return Name; };
protected:
	void Tick(float DeltaTime);
	friend class World;
};

class Component : public Object, public ITickable
{
private:
	ETickGroup TickGroup;
	class Entity* Owner;
public:
	Component(String inName, class Entity* inOwner);
	~Component();

	virtual void Tick(float DeltaTime, ETickGroup TickGroup) override {};
	virtual void SetTickGroup(ETickGroup inTickGroup = ETickGroup::Tick) override;
	virtual void RemoveFromTickGroup() override;

	class Entity* GetOwner() const { return Owner; };
};


class WorldPresentComponent : public Component
{
	using Component::Component;
private:
	Transform2D LocalTransform;
public:
	Transform2D GetWorldTransform() const;
};

class VisualComponent : public WorldPresentComponent
{
	using WorldPresentComponent::WorldPresentComponent;
private:
	int SortOrder = 0;
public:
	virtual void Draw(ID2D1BitmapRenderTarget* renderTarget) {};

	void SetSortOrder(int inOrder) { SortOrder=inOrder; };
	int GetSortOrder() const { return SortOrder; }

};

class Entity : public Object, public ITickable
{
	using Object::Object;
private:
	ETickGroup TickGroup;
	Transform2D Transform;
	LinkedList<Component*> Components;
public:

	Entity(String inName, World* inWorld);
	virtual ~Entity();

	template<typename T>
	T* CreateComponent(String Name)
	{
		T* Result = new T(Name, this);
		RegisterComponent(Result);
		return Result;
	};

	LinkedList<Component*>& GetComponents() { return Components; };


	void RegisterComponent(class Component* inComponent);
	void UnregisterComponent(class Component* inComponent);

	void SetPosition(const Vector2D& newPosition) { Transform.SetPosition(newPosition); };
	Vector2D GetPosition() const { return Transform.GetPosition(); };
	Transform2D GetTransform() const { return Transform; };	

	virtual void Tick(float DeltaTime, ETickGroup TickGroup) override {};
	virtual void SetTickGroup(ETickGroup inTickGroup = ETickGroup::Tick) override;
	virtual void RemoveFromTickGroup() override;
};
