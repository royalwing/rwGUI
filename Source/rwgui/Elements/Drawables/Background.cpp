#include "Background.h"

Background::Background(String name, Color inColor)
	: Drawable(name)
	, color(inColor)
{
	bInteractive = false;
}

void Background::Init()
{
	
}

void Background::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (brush == nullptr) renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &brush);
	renderTarget->FillRectangle(GetBounds().ToD2DRect(), brush);
	if (backgroundImage != nullptr) renderTarget->DrawBitmap(backgroundImage, GetBounds().ToD2DRect(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, Bounds(0, 0, backgroundImage->GetSize().width, backgroundImage->GetSize().height).ToD2DRect());
}

void Background::Update(float DeltaTime)
{
	SetPosition(GetOuterBounds(IsNonClient()).Pos.x, GetOuterBounds(IsNonClient()).Pos.y);
	SetSize(GetOuterBounds(IsNonClient()).Size.x, GetOuterBounds(IsNonClient()).Size.y);
}

void Background::SetBackgroundImage(String backgroundImagePath)
{
	if (backgroundImagePath.IsEmpty())
		backgroundImage = nullptr;
	else
		backgroundImage = MAKEBITMAP(backgroundImagePath);
}

void Background::SetBackgroundColor(Color BackgroundColor)
{
	color = BackgroundColor;
}

Color Background::GetBackgroundColor() const
{
	return color;
}
