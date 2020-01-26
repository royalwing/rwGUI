#pragma once
#include "Engine/Object.h"

class DynamicSpriteComponent : public VisualComponent
{
	using VisualComponent::VisualComponent;
private:
	ID2D1BitmapRenderTarget* RenderTarget = nullptr;
	Vector2D Size = Vector2D(32, 32);
	void OnResize();
public:
	DynamicSpriteComponent(String Name, Entity* Owner);

	void SetSize(Vector2D inSize);
	Vector2D GetSize() const;

	virtual void Draw(ID2D1BitmapRenderTarget* renderTarget) override;

	virtual void OnDynamicDraw(ID2D1BitmapRenderTarget* renderTarget) {}; 
};
