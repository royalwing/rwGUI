#pragma once
#include "Elements/Drawables/Drawable.h"

class World
{
private:
	World() {};
	World(class Engine* pEngine);
	~World();
	class Engine* _engine = nullptr;

	void Tick(float DeltaTime);
	
	friend class Engine;
public:
	class Engine* GetEngine() const { return _engine; }
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
public:

	Viewport(String Name, World* inWorld);

	virtual void Init() override;
	virtual void Update(float DeltaTime) override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;

	class World* GetWorld() const { return pWorld; };
};