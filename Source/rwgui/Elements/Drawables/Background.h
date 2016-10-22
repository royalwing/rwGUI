#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Background : public Drawable
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	Color color;
	ID2D1Bitmap* backgroundImage = nullptr;
public:
	Background(char* name, Color inColor);
	virtual void Init() override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Update(float DeltaTime) override;
	
	void SetBackgroundImage(char* backgroundImagePath);
	void SetBackgroundColor(Color BackgroundColor);
};

#endif