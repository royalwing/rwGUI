#include "Engine.h"
#include "Common/DebugHelpers.h"

World::World(Engine* pEngine)
	: _engine(pEngine)
{
	
}

World::~World()
{
	GetEngine()->RemoveWorld(this);
}

void World::Tick(float DeltaTime)
{

}

World* Engine::CreateWorld()
{
	World* pNewWorld = new World(this);
	Worlds.Add(pNewWorld);
	return pNewWorld;
}

void Engine::RemoveWorld(World* pWorld)
{
	Worlds.Remove(pWorld);
}

void Engine::Tick(float DeltaTime)
{
	for(World* pWorld : Worlds)
		pWorld->Tick(DeltaTime);
}

void Engine::Stop()
{

}

Viewport::Viewport(String Name, World* inWorld)
	: Drawable(Name), pWorld(inWorld)
{
	SetPadding(2, 2, 2, 2);


}

void Viewport::Init()
{
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_SIZE_F size;
	size.height = 512;
	size.width = 512;
	GetApplication()->GetRenderer()->GetRenderTarget()->CreateCompatibleRenderTarget(size, &viewportRT);
}

void Viewport::Update(float DeltaTime)
{
	Vector2D Size = GetOuterBounds(false).Size;
	SetSize(Size.x, Size.y);
}

void Viewport::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	ID2D1Bitmap* Bitmap = nullptr;
	viewportRT->BeginDraw();
	viewportRT->Clear(Color(0.05f, 0.05f, 0.05f,1.0f).ToD2D1ColorF());
	viewportRT->GetBitmap(&Bitmap);
	viewportRT->EndDraw();
	renderTarget->DrawBitmap(Bitmap, GetBounds().ToD2DRect());
}
