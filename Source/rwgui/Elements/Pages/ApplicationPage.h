#ifndef APPLICATIONPAGE_H
#define APPLICATIONPAGE_H

#include <rwgui.h>
#include <Elements/Drawables/Drawable.h>

class RWGUI_API ApplicationPage
{
private:
	Application* App;
	String pageTitle;
public:
	List<Drawable*> Elements;
	ApplicationPage();
	~ApplicationPage();
	void AddElement(Drawable* inElement);
	virtual Bounds GetBounds(bool bNonClient = false);
	virtual Bounds GetClientBounds() { return GetBounds(); };
	Application* GetApplication();
	Drawable* GetDrawableAtPosition(Vector2D Position);
	void Init();
	virtual void OnInit() {};
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget);
	void InternalUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	void SetApp(Application* nApp);
	virtual void SetTitle(String newtitle);
	virtual void OnWindowResize(Vector2D inSize);

	String GetTitle();

	virtual void BuildPage() {};

	friend Application;
};

#endif