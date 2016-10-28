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
	RECT Padding;
	ApplicationPage* appPage;
	Drawable* Outer;
	char* name;
public:
	bool bInteractive;
	bool bIsNonClient;
	float LayoutScale = 1.0f;
	int zOrder = 0;
	std::vector<Drawable*> Elements; // Dont touch this array directly. Use AddChild() instead.

	Drawable(char* newname);
	void AddChild(Drawable* child);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget);
	virtual Drawable* GetOuter();
	void InternalInit();
	virtual void Init() {};
	virtual void InternalUpdate(float DeltaTime);
	virtual void Update(float DeltaTime) {};
	char* GetName();
	virtual Bounds GetBounds(bool bNonClient = false);
	virtual Bounds GetSelectionBounds();
	virtual Bounds GetOuterBounds(bool bNonClient = false);
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };
	Drawable* GetDrawableAtPosition(Vector2D Position);
	virtual void OnMouseEnter() {};
	virtual void OnMouseLeave() {};
	virtual void OnMousePress() {};
	virtual void OnMouseRelease() {};
	virtual void OnMouseClick() {};
	virtual void OnGlobalEvent(EGlobalEvent eventType) {};
	virtual HCURSOR GetCursor() { return LoadCursor(nullptr, IDC_ARROW); };
	bool IsInteractive() { return bInteractive; };
	bool IsNonClient() { return bIsNonClient; };

	void SetPosition(float x, float y) { Position.x = x; Position.y = y; };
	void SetSize(float x, float y) { Size.x = x; Size.y = y; };
	void SetPadding(RECT nPadding) { Padding = nPadding; };
	void SetPadding(int top, int left, int bottom, int right) { Padding.left = left; Padding.right = right; Padding.bottom = bottom; Padding.top = top; };

	ApplicationPage* GetAppPage();
	Application* GetApplication();

	friend ApplicationPage;
};


#endif