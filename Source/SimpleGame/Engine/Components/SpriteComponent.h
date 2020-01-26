#pragma once
#include "../Object.h"

class SpriteComponent : public VisualComponent
{
	using VisualComponent::VisualComponent;
private:
	ID2D1Bitmap* SpriteBitmap = nullptr;
	String lastSpriteSource;
	Vector2D Size = Vector2D(100, 100);
	float Opacity = 1.0f;
public:
	SpriteComponent(String Name, Entity* Owner, String SpriteSource);

	void SetSpriteBitmap(String newSpriteSource);

	virtual void Draw(ID2D1BitmapRenderTarget* renderTarget) override;

	void SetSpriteSize(const Vector2D inSize) { Size = inSize; };
	Vector2D GetSpriteSize() const { return Size; };
	void SetSpriteOpacity(float inOpacity) { Opacity = inOpacity; };
	float GetSpriteOpacity() const { return Opacity; };
};
