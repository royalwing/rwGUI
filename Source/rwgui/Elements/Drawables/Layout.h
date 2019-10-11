#ifndef LAYOUT_H
#define LAYOUT_H

#include <Elements/Drawables/Drawable.h>

enum ELayoutType
{
	LayoutType_ASIS = 0, // Means all children in this layout will save their locations and sizes unchanged
	LayoutType_VERTICAL, // Means all children will be arranged in a column
	LayoutType_HORIZONTAL, // Means all children will be arranged in a row
};

enum ELayoutVertialContentAlignment
{
	LVCA_Default = 0,
	LVCA_Top,
	LVCA_Middle,
	LVCA_Bottom
};

enum ELayoutHorizontalContentAlignment
{
	LHCA_Default = 0,
	LHCA_Left,
	LHCA_Middle,
	LHCA_Right
};

class RWGUI_API Layout : public Drawable
{
private:
	ELayoutType LayoutType;
	RECT contentPadding;
	ELayoutVertialContentAlignment contentVerticalAlignment = LVCA_Default;
	ELayoutHorizontalContentAlignment contentHorizontalAlignment = LHCA_Default;
public:
	Layout(String name, ELayoutType Type);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Update(float DeltaTime) override;
	void SetContentPadding(RECT nPadding);
	void SetContentPadding(int top, int left, int bottom, int right);
	void SetContentVerticalAlignment(ELayoutVertialContentAlignment newVerticalAlignment);
	void SetContentHorizontalAlignment(ELayoutHorizontalContentAlignment newHorizontalAlignment);
};

#endif // LAYOUT