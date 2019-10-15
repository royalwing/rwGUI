#pragma once
#include "Elements/Drawables/Drawable.h"
#include "Object.h"

class RenderPrimitive
{
public:
	Color color;
	Vector2D Position;
	float Rotation;
};

class World
{
private:
	World() {};
	World(class Engine* pEngine);
	~World();
	class Engine* _engine = nullptr;

	void Tick(float DeltaTime);

	List<class Entity*> Entities;

	friend class Engine;
public:
	class Engine* GetEngine() const { return _engine; }

	void Draw(ID2D1BitmapRenderTarget* RenderTarget);

	template<typename T>
	T* SpawnEntity(String Name)
	{
		T* Result = new T(Name, this);
		Entities.Add(Result);
		return Result;
	}

	void DestroyEntity(class Entity* inEntity)
	{
		Entities.Remove(inEntity);
		delete inEntity;
	}
};

class Engine
{
private:
	Engine() {};
	~Engine() {};
	List<World*> Worlds;
public:
	static Engine* Get() { static Engine instance; return &instance; };

	World* CreateWorld();
	void RemoveWorld(World* pWorld);

	virtual void Tick(float DeltaTime);
	virtual void Stop();
};

class Viewport : public Drawable
{
private:
	class World* pWorld = nullptr;;
	ID2D1BitmapRenderTarget* viewportRT = nullptr;
	Transform2D Transform;
public:

	Viewport(String Name, World* inWorld);

	virtual void Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;

	class World* GetWorld() const { return pWorld; };

	void SetPosition(Vector2D inPosition) { Transform.Position = inPosition; };
	Vector2D GetPosition() const { return Transform.Position; };

	void SetRotation(float inRotation) { Transform.Rotation = inRotation; };
	float GetRotation() const { return Transform.Rotation; };

	void SetScale(Vector2D inScale) { Transform.Scale = inScale; };
	Vector2D GetScale() const { return Transform.Scale; };
};