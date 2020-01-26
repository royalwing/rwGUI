#include "DynamicSpriteComponent.h"
#include "rwgui.h"

void DynamicSpriteComponent::OnResize()
{
	if (RenderTarget != nullptr)
	{
		RenderTarget->Release();
	}
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	D2D1_SIZE_F size;
	size.height = Size.y;
	size.width = Size.x;
	GApplication->GetRenderer()->GetRenderTarget()->CreateCompatibleRenderTarget(size, &RenderTarget);
}

DynamicSpriteComponent::DynamicSpriteComponent(String Name, Entity* Owner)
	: VisualComponent(Name, Owner)
{
	OnResize();
}

void DynamicSpriteComponent::SetSize(Vector2D inSize)
{
	Size = inSize;
	OnResize();
}

Vector2D DynamicSpriteComponent::GetSize() const
{
	return Size;
}

void DynamicSpriteComponent::Draw(ID2D1BitmapRenderTarget* renderTarget)
{
	ID2D1Bitmap* Bitmap = nullptr;
	RenderTarget->BeginDraw();
	RenderTarget->Clear(Color(0,0,0,0).ToD2D1ColorF());
	RenderTarget->SetTransform(Transform2D().ToD2D1Matrix());
	OnDynamicDraw(RenderTarget);
	RenderTarget->GetBitmap(&Bitmap);
	RenderTarget->EndDraw();
	Bounds bounds = Bounds(-Size.x*0.5f, -Size.y*0.5f, Size.x, Size.y);
	renderTarget->DrawBitmap(Bitmap, bounds.ToD2DRect(), 1.0f);
}

