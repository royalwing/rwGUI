#pragma once
#include "../Object.h"

class SpriteComponent : public VisualComponent
{
private:
	ID2D1Bitmap* SpriteBitmap = nullptr;
	String lastSpriteSource;
public:
	SpriteComponent(String Name, Entity* Owner, String SpriteSource);

	void SetSpriteBitmap(String newSpriteSource);

	virtual void Draw(ID2D1BitmapRenderTarget* renderTarget) override;
};
