#pragma once
#include "Common/String.h"
#include "Common/rwmath.h"

class Object
{
private:
	String Name;
	class World* OuterWorld = nullptr;

	Object() = delete;
public:
	Object(String inName, class World* inWorld);

	typedef void(*OnTickDelegate)(Object* Object, float DeltaTime);
	OnTickDelegate TickDelegate;

	virtual void OnTick(float DeltaTime) {};

	class World* GetWorld() const { return OuterWorld; };
protected:
	void Tick(float DeltaTime);
	friend class World;
};

class Component : public Object
{
private:
	class Entity* Owner;
public:
	Component(String inName, class Entity* inOwner);
	virtual ~Component() {};

	class Entity* GetOwner() const { return Owner; };
};

class VisualComponent : public Component
{
	using Component::Component;
private:
	Transform2D LocalTransform;
	int SortOrder = 0;
public:
	Transform2D GetWorldTransform() const;

	virtual void Draw(ID2D1BitmapRenderTarget* renderTarget) {};

	void SetSortOrder(int inOrder) { SortOrder=inOrder; };
	int GetSortOrder() const { return SortOrder; }
};

class Entity : public Object
{
	using Object::Object;
private:
	Transform2D Transform;
	LinkedList<Component*> Components;
public:
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

	void SetPosition(const Vector2D& newPosition)
	{
		Transform.SetPosition(newPosition);
	};
	Vector2D GetPosition() const { return Transform.GetPosition(); };

	Transform2D GetTransform() const { return Transform; };
};
