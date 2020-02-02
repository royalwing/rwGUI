#include "Engine.h"
#include "Common/DebugHelpers.h"

World::World(Engine* pEngine)
	: _engine(pEngine)
{
	
}

World::~World()
{
	for (auto Itr = Entities.Itr(); !Itr.IsFirst(); Itr = Itr.Next())
	{
		delete Itr.Get();
	}
	GetEngine()->RemoveWorld(this);
}

void World::Tick(float DeltaTime)
{
	CurrentDeltaSeconds = DeltaTime;
	for (auto Itr = Entities.Itr(); Itr.IsValid(); Itr = Itr.Next())
	{
		Itr->Tick(DeltaTime);
		if (!Itr.IsValid()) continue;// Entity may be deleted while tick
		for (auto CompItr = Itr->GetComponents().Itr(); CompItr.IsValid(); CompItr = CompItr.Next())
		{
			CompItr->Tick(DeltaTime);
		}
	}
}

void World::Draw(ID2D1BitmapRenderTarget* RenderTarget)
{
	D2D1_MATRIX_3X2_F ViewTransform;
	RenderTarget->GetTransform(&ViewTransform);
	List<VisualComponent*> ComponentsToDraw;
	for (auto Itr = Entities.Itr(); Itr.IsValid(); Itr = Itr.Next())
	{
		for(auto CompItr = Itr->GetComponents().Itr(); CompItr.IsValid(); CompItr = CompItr.Next())
		{
			if (VisualComponent * VisComp = dynamic_cast<VisualComponent*>(CompItr.Get()))
			{
				ComponentsToDraw.Add(VisComp);
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
	bInteractive = true;
	SetPadding(2, 2, 2, 2);
	ClearColor = Color(0.05f, 0.05f, 0.05f, 1.0f);
}

void Viewport::Init()
{
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_SIZE_F size;
	Vector2D Size = GetOuterBounds(false).Size;
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

void Viewport::OnMouseMove(const Vector2D& Position)
{
	Vector2D Pos = Position;
	LocalMousePosition = Pos;
}

Vector2D Viewport::GetLocalCursorPosition() const
{
	return LocalMousePosition;
}

Vector2D Viewport::GetWorldCursorPosition()
{
	Vector2D Local = GetLocalCursorPosition();
	return ScreenToWorld(Local);
}

Vector2D Viewport::ScreenToWorld(Vector2D WorldPosition)
{
	WorldPosition -= GetBounds(true).Size / 2;
	WorldPosition.y *= -1;
	WorldPosition += GetWorldPosition();
	return WorldPosition;
}

Vector2D Viewport::WorldToScreen(Vector2D ScreenPosition)
{
	ScreenPosition -= GetWorldPosition();
	ScreenPosition.y *= -1;
	ScreenPosition += GetBounds(true).Size / 2;
	return ScreenPosition;
}

HCURSOR Viewport::GetCursor()
{
	return nullptr;
}
