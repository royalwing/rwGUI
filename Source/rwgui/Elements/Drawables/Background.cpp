#include "Background.h"

Background::Background(char* name, Color inColor)
	: Drawable(name)
	, color(inColor)
{
	bInteractive = false;
}

void Background::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (brush == nullptr) renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &brush);
	renderTarget->FillRectangle(GetOuterBounds().ToD2DRect(), brush);
	if (backgroundImage != nullptr) renderTarget->DrawBitmap(backgroundImage, GetOuterBounds().ToD2DRect(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, Bounds(0, 0, backgroundImage->GetSize().width, backgroundImage->GetSize().height).ToD2DRect());
}

void Background::Update(float DeltaTime)
{
	SetPosition(0, 0);
	SetSize(GetOuterBounds().Size.x,GetOuterBounds().Size.y);
}

void Background::SetBackgroundImage(char * backgroundImagePath)
{
	if (strlen(backgroundImagePath) == 0) backgroundImage = nullptr;
	else
	{
		backgroundImage = MAKEBITMAP(backgroundImagePath);
	}
}
