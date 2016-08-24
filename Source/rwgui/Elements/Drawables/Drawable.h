#ifndef DRAWABLE_H
#define DRAWABLE_H

class RWD2D;
class Application;


#include <Elements/Pages/ApplicationPage.h>

class RWGUI_API Drawable
{
private:
	ApplicationPage* appPage;
	Drawable* Outer;
	std::vector<Drawable*> Elements;
	char* name;
public:
	bool bInteractive;

	Drawable(char* newname);
	void AddChild(Drawable* child);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget);
	virtual Drawable* GetOuter();
	char* GetName();
	virtual Bounds GetBounds();
	virtual Bounds GetSelectionBounds() { return GetBounds(); };
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };
	Bounds GetOuterBounds();
	Bounds GetAbsoluteBounds();
	Drawable* GetDrawableAtPosition(Vector2D Position);
	virtual void OnMousePress() {};
	virtual void OnMouseRelease() {};
	virtual void OnMouseClick() {};
	bool IsInteractive() { return bInteractive; };

	ApplicationPage* GetAppPage();
	Application* GetApplication();

	friend ApplicationPage;
};


#endif