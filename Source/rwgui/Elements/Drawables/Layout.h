#ifndef LAYOUT_H
#define LAYOUT_H

#include <Elements/Drawables/Drawable.h>

enum ELayoutType
{
	LayoutType_ASIS, // Means all children in this layout will save their locations and sizes unchanged
	LayoutType_VERTICAL, // Means all children will be arranged in a column
	LayoutType_HORIZONTAL, // Means all children will be arranged in a row
};

class RWGUI_API Layout : public Drawable
{
private:
	ELayoutType LayoutType;
	RECT contentPadding;
public:
	Layout(char* name, ELayoutType Type);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Update(float DeltaTime) override;
	void SetContentPadding(RECT nPadding);
};

#endif // LAYOUT