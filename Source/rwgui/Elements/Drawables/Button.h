#ifndef BUTTON_H
#define BUTTON_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Button : public Drawable
{
public:
	typedef void(*OnButtonPressedDelegate)(Application* app);
private:
	Vector2D Position;
	Vector2D Size;
	OnButtonPressedDelegate OnButtonPressed = nullptr;
public:
	Button(char* name, Bounds bounds = Bounds(0, 0, 200, 35), OnButtonPressedDelegate btnPressed = nullptr);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() override;
	virtual void OnMouseClick() override;
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };

};


#endif