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
	wchar_t* Caption = L"";
	OnButtonPressedDelegate OnButtonPressed = nullptr;
	bool bPressed = false;
public:
	Button(char* name, std::string caption, Bounds bounds = Bounds(0, 0, 200, 35), OnButtonPressedDelegate btnPressed = nullptr);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() override;
	virtual void OnMousePress() { bPressed = true; };
	virtual void OnMouseRelease() { bPressed = false; };
	virtual void OnMouseClick() override;
	virtual void OnGlobalEvent(EGlobalEvent eventType) override { if (eventType == MOUSEBUTTONRELEASED) bPressed = false; };
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };

};


#endif