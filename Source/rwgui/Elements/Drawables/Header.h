#ifndef HEADER_H
#define HEADER_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Header : public Drawable
{
private:
	ID2D1LinearGradientBrush* brush = nullptr;
	ID2D1SolidColorBrush* textBrush = nullptr;
	IDWriteTextFormat* textFormat = nullptr;
	char* headerTitle = "";
public:
	Header(char* name, char* title);
	virtual Bounds GetBounds() override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual int GetDrawableNCObjectType() { return HTCAPTION; };
};



#endif