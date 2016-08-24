#include "Button.h"
#include "../../rwgui.h"

Button::Button(char* name, Bounds bounds, Button::OnButtonPressedDelegate OnPress)
	: Drawable(name)
{
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
	Drawable* curHovered = GetApplication()->GetCurrentHoveredDrawable();
	if (curHovered != nullptr &&  curHovered == this)
	{

	}
}

Bounds Button::GetBounds()
{
	return Bounds(Position, Size);
}