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
	renderTarget->FillRectangle(GetAbsoluteBounds().ToD2DRect(), brush);
}