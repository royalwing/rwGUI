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
}

void Button::OnMouseClick()
{
	if (OnButtonPressed != nullptr) OnButtonPressed(GetApplication());
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
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(0.88f, 0.88f, 0.88f)));
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(0.6f,0.6f,0.6f)));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(0.0f, 0.0f, 0.0f)));
	} else
	if (curHovered != nullptr &&  curHovered == this)
	{
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(0.8f, 0.8f, 0.8f)));
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
	}
	else {
		renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(0.7f, 0.7f, 0.7f)));
		renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
		renderTarget->DrawText(Caption, wcslen(Caption), textFormat, Bounds(b.Pos.x + b.Size.x / 2 - metrics.width / 2, b.Pos.y + b.Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
	}

}

Bounds Button::GetBounds()
{
	return Bounds(Position, Size);
}