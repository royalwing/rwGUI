#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <rwapi.h>
#include <Common/rwmath.h>
#include <rwd2d.h>
#include <vector>

#pragma warning(disable : 4244 4311 4312 4838 4244)

class ApplicationPage;

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

enum RWGUI_API EBorderType
{
	Top,
	Bottom,
	Left,
	Right,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft
};

class RWGUI_API Border : public Drawable
{
private:
	ID2D1SolidColorBrush* borderBrush = nullptr;
public:
	EBorderType borderType;
	Color borderColor;
	int borderWidth;
	Border(char* name,EBorderType borderType, Color color,int borderWidth = 1);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() override;
	Bounds GetSelectionBounds() override;
	virtual int GetDrawableNCObjectType() override;
};

class RWGUI_API Button : public Drawable
{
public:
	typedef void(*OnButtonPressedDelegate)(Application* app);
private:
	Vector2D Position;
	Vector2D Size;
	OnButtonPressedDelegate OnButtonPressed = nullptr;
public:
	Button(char* name,Bounds bounds = Bounds(0,0,200,35), OnButtonPressedDelegate btnPressed = nullptr);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() override;
	virtual void OnMouseClick() override;
	virtual int GetDrawableNCObjectType() { return HTCLIENT; };

};

class RWGUI_API Header : public Drawable
{
private:
	ID2D1LinearGradientBrush* brush = nullptr;
	ID2D1SolidColorBrush* textBrush = nullptr;
	IDWriteTextFormat* textFormat = nullptr;
	char* headerTitle = "";
public:
	Header(char* name, char* title);
	virtual Bounds GetBounds() override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual int GetDrawableNCObjectType() { return HTCAPTION; };
};

class RWGUI_API Background : public Drawable
{
private:
	ID2D1SolidColorBrush* brush = nullptr;
	Color color;
public:
	Background(char* name,Color inColor);
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual Bounds GetBounds() { return Bounds(0,0,GetOuterBounds().Size.x,GetOuterBounds().Size.y); };
};

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

class RWGUI_API BasePage : public ApplicationPage
{
public:
	BasePage(char* pageTitle,Color backgroundColor = Color(0.9f, 0.9f, 0.9f),Color borderColor = Color(0.75f, 0.75f, 0.75f),int borderWidth = 1);
};

#endif
