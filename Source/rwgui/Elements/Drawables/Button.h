#ifndef BUTTON_H
#define BUTTON_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Button : public Drawable
{
public:
	typedef void(*OnButtonPressedDelegate)(Application* app);
private:
	Color BackgroundColor;
	String Caption;
	OnButtonPressedDelegate OnButtonPressed = nullptr;
	ID2D1Bitmap* background = nullptr;
	int fontSize = 16;
	bool bPressed = false;
public:
	enum EBackgroundAlignment
	{
		BA_AsIs = 0,
		BA_StretchToFit,
		BA_ScaleToFit
	} BackgroundAlignment = BA_AsIs;

	Button(String name, OnButtonPressedDelegate btnPressed = nullptr);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual void OnMousePress() { bPressed = true; };
	virtual void OnMouseRelease() { bPressed = false; };
	virtual void OnMouseLeave() { bPressed = false; };
	virtual void OnMouseClick() override;
	virtual void OnGlobalEvent(EGlobalEvent eventType) override { if (eventType == MOUSEBUTTONRELEASED) bPressed = false; };
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };
	virtual HCURSOR GetCursor() override;
	
	void SetBackgroundColor(Color bgColor);
	void SetBackgroundImage(String bgImagePath);
	void SetBackgroundImage(int ResourceID);
	void SetCaption(String caption);
	void SetFontSize(int nFontSize) { fontSize = nFontSize; };
	int GetFontSize() { return fontSize; };
};


#endif