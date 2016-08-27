#ifndef DRAWABLE_H
#define DRAWABLE_H

class RWD2D;
class Application;

#include <Resources/ResourceManager.h>
#include <Elements/Pages/ApplicationPage.h>

class RWGUI_API Drawable
{
private:
	Vector2D Position;
	Vector2D Size;
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
	virtual void InternalUpdate(float DeltaTime);
	virtual void Update(float DeltaTime) {};
	char* GetName();
	virtual Bounds GetBounds();
	virtual Bounds GetSelectionBounds() { return GetBounds(); };
	virtual Bounds GetOuterBounds();
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };
	Drawable* GetDrawableAtPosition(Vector2D Position);
	virtual void OnMousePress() {};
	virtual void OnMouseRelease() {};
	virtual void OnMouseClick() {};
	virtual void OnGlobalEvent(EGlobalEvent eventType) {};
	virtual HCURSOR GetCursor() { return LoadCursor(nullptr, IDC_ARROW); };
	bool IsInteractive() { return bInteractive; };

	void SetPosition(float x, float y) { Position.x = x; Position.y = y; };
	void SetSize(float x, float y) { Size.x = x; Size.y = y; };

	ApplicationPage* GetAppPage();
	Application* GetApplication();

	friend ApplicationPage;
};


#endif