#ifndef BUTTON_H
#define BUTTON_H

#include <Elements/Drawables/Drawable.h>
#include <Common/Events.h>

class RWGUI_API Button : public Drawable
{
private:
	Color BackgroundColor;
	String Caption;
	ID2D1Bitmap* background = nullptr;
	int fontSize = 16;
	bool bPressed = false;
public:
	MulticastEvent<Button*> OnButtonPressedEvent;

	enum EBackgroundAlignment
	{
		BA_AsIs = 0,
		BA_StretchToFit,
		BA_ScaleToFit
	} BackgroundAlignment = BA_AsIs;

	Button(String name,std::function<void(Button*)> btnPressed = nullptr);

	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual void OnMousePress() { bPressed = true; };
	virtual void OnMouseRelease() { bPressed = false; };
	virtual void OnMouseLeave() { bPressed = false; };
	virtual void OnMouseClick() override;
	virtual void OnGlobalEvent(EGlobalEvent eventType) override { if (eventType == MOUSEBUTTONRELEASED) bPressed = false; };
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };
	virtual HCURSOR GetCursor() override;
	
	void SetBackgroundColor(const Color& bgColor);
	void SetBackgroundImage(String bgImagePath);
	void SetBackgroundImage(int ResourceID);
	void SetCaption(String caption);
	void SetFontSize(int nFontSize) { fontSize = nFontSize; };
	int GetFontSize() { return fontSize; };
};


#endif