#ifndef BORDER_H
#define BORDER_H

#include <Elements/Drawables/Drawable.h>

enum RWGUI_API EBorderType
{
	Top,
	Bottom,
	Left,
	Right,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft
};

class RWGUI_API Border : public Drawable
{
private:
	ID2D1SolidColorBrush* borderBrush = nullptr;
public:
	EBorderType borderType;
	Color borderColor;
	int borderWidth;
	Border(char* name, EBorderType borderType, Color color, int borderWidth = 1);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() override;
	Bounds GetSelectionBounds() override;
	virtual int GetDrawableNCObjectType() override;
};

#endif BORDER_H