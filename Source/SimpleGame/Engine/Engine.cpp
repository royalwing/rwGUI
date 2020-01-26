﻿#include "Engine.h"
#include "Common/DebugHelpers.h"

World::World(Engine* pEngine)
	: _engine(pEngine)
{
	
}

World::~World()
{
	for (Entity* pEntity : Entities)
	{
		delete pEntity;
	}
	GetEngine()->RemoveWorld(this);
}

void World::Tick(float DeltaTime)
{
	for (Entity* pEntity : Entities)
	{
		pEntity->Tick(DeltaTime);
		for (Component* pComp : pEntity->GetComponents())
		{
			if (VisualComponent * visComp = dynamic_cast<VisualComponent*>(pComp))
			{
				visComp->Tick(DeltaTime);
			}
		}
	}
}

void World::Draw(ID2D1BitmapRenderTarget* RenderTarget)
{
	D2D1_MATRIX_3X2_F ViewTransform;
	RenderTarget->GetTransform(&ViewTransform);
	List<VisualComponent*> ComponentsToDraw;
	for(Entity* pEntity : Entities)
	{
		for(Component* pComp : pEntity->GetComponents())
		{
			if(VisualComponent* visComp = dynamic_cast<VisualComponent*>(pComp))
			{
				ComponentsToDraw.Add(visComp);
			}
		}
	}
	ComponentsToDraw.Sort([](VisualComponent* const& A, VisualComponent* const& B)->bool { return A->GetSortOrder() > B->GetSortOrder(); });
	for(VisualComponent* Component : ComponentsToDraw)
	{
		Transform2D ComponentTransform = Component->GetWorldTransform();
		ComponentTransform.Position.y *= -1;
		D2D1_MATRIX_3X2_F WorldMatrix = ViewTransform * ComponentTransform.ToD2D1Matrix();
		RenderTarget->SetTransform(WorldMatrix);
		Component->Draw(RenderTarget);
	}
	RenderTarget->SetTransform(ViewTransform);
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

void Viewport::ResizeViewport(Vector2D InSize)
{
	if(viewportRT!=nullptr)
	{
		viewportRT->Release();
	}
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_SIZE_F size;
	size.height = InSize.y;
	size.width = InSize.x;
	GetApplication()->GetRenderer()->GetRenderTarget()->CreateCompatibleRenderTarget(size, &viewportRT);
}

Viewport::Viewport(String Name, World* inWorld)
	: Drawable(Name), pWorld(inWorld)
{
	SetPadding(2, 2, 2, 2);
	ClearColor = Color(0.05f, 0.05f, 0.05f, 1.0f);
}

void Viewport::Init()
{
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_SIZE_F size;
	Vector2D Size = GetOuterBounds(true).Size;
	size.height = Size.y;
	size.width = Size.x;
	ResizeViewport(Size);
}

void Viewport::Update(float DeltaTime)
{
	Vector2D Size = GetOuterBounds(false).Size;
	SetSize(Size.x, Size.y);
}

void Viewport::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	Bounds Bounds = GetBounds();
	ID2D1Bitmap* Bitmap = nullptr;
	viewportRT->BeginDraw();
	viewportRT->Clear(ClearColor.ToD2D1ColorF());
	Transform2D T = Transform;
	T.Position.x -= Bounds.Size.x / 2;
	T.Position.y *= -1;
	T.Position.y -= Bounds.Size.y / 2;
	viewportRT->SetTransform(T.Inverse().ToD2D1Matrix());
	if (GetWorld() != nullptr)
		GetWorld()->Draw(viewportRT);
	viewportRT->GetBitmap(&Bitmap);
	viewportRT->EndDraw();
	renderTarget->DrawBitmap(Bitmap, Bounds.ToD2DRect());
}

void Viewport::OnWindowResize(const Vector2D& inSize)
{
	Drawable::OnWindowResize(inSize);
	ResizeViewport(inSize);
}
