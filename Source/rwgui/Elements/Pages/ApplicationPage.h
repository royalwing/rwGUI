#ifndef APPLICATIONPAGE_H
#define APPLICATIONPAGE_H

#include <rwgui.h>
#include <Elements/Drawables/Drawable.h>

class RWGUI_API ApplicationPage
{
private:
	Application* App;
	std::vector<Drawable*> Elements;
public:
	ApplicationPage();
	~ApplicationPage();
	void AddElement(Drawable* inElement);
	Bounds GetBounds();
	Application* GetApplication();
	Drawable* GetDrawableAtPosition(Vector2D Position);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget);
	void SetApp(Application* nApp);


	friend Application;
};

#endif