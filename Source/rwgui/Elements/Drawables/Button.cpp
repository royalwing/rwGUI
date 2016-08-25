#include "Button.h"
#include "../../rwgui.h"

Button::Button(char* name, std::string caption,Bounds bounds, Button::OnButtonPressedDelegate OnPress)
	: Drawable(name)
{
	int captionLen = caption.size();
	Caption = new wchar_t[captionLen];
	Caption[captionLen] = '\0';
	mbstowcs(Caption, caption.c_str(), caption.size());
	bInteractive = true;
	OnButtonPressed = OnPress;
	Position = bounds.Pos;
	Size = bounds.Size;
	BackgroundColor = Color(1.0f, 1.0f, 1.0f);
}

void Button::OnMouseClick()
{
	if (OnButtonPressed != nullptr) OnButtonPressed(GetApplication());
}

void Button::SetBackgroundColor(Color bgColor)
{
	BackgroundColor = bgColor;
}

void Button::SetBackgroundImage(char * bgImagePath)
{
	if (strlen(bgImagePath) == 0) background = nullptr;
	background = MAKEBITMAP(bgImagePath);
}

void Button::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	IDWriteTextFormat* textFormat = MAKETEXTFORMAT("Arial", 16, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_EXTRA_EXPANDED);
	IDWriteTextLayout* textLayout;
	d2d->GetWriteFactory()->CreateTextLayout(Caption, wcslen(Caption), textFormat, GetBounds().Size.x, GetBounds().Size.y, &textLayout);
	DWRITE_TEXT_METRICS metrics;
	textLayout->GetMetrics(&metrics);
	Bounds b = GetBounds();
	Drawable* curHovered = GetApplication()->GetCurrentHoveredDrawable();
	if (bPressed)
	{
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.87f));
		if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 1.0f,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, Bounds(0,0,background->GetSize().width, background->GetSize().height).ToD2DRect());
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.6f));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(0.0f, 0.0f, 0.0f)));
	} else
	if (curHovered != nullptr &&  curHovered == this)
	{
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.8f));
		if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 0.9f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, Bounds(0, 0, background->GetSize().width, background->GetSize().height).ToD2DRect());
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
	}
	else {
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.7f));
		if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, Bounds(0, 0, background->GetSize().width, background->GetSize().height).ToD2DRect());
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
	}

}

Bounds Button::GetBounds()
{
	return Bounds(Position, Size);
}