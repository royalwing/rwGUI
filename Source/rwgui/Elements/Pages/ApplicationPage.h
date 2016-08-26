#ifndef APPLICATIONPAGE_H
#define APPLICATIONPAGE_H

#include <rwgui.h>
#include <Elements/Drawables/Drawable.h>

class RWGUI_API ApplicationPage
{
private:
	Application* App;
	std::vector<Drawable*> Elements;
	char* pageTitle = "";
public:
	ApplicationPage();
	~ApplicationPage();
	void AddElement(Drawable* inElement);
	Bounds GetBounds();
	Application* GetApplication();
	Drawable* GetDrawableAtPosition(Vector2D Position);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget);
	void InternalUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	void SetApp(Application* nApp);
	virtual void SetTitle(char* newtitle);

	char* GetTitle();

	virtual void BuildPage() {};

	friend Application;
};

#endif