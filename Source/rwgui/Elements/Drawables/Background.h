#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Background : public Drawable
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	Color color;
public:
	Background(char* name, Color inColor);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() { return Bounds(0, 0, GetOuterBounds().Size.x, GetOuterBounds().Size.y); };
};

#endif