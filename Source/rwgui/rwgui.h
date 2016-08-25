#ifndef RWGUI_H
#define RWGUI_H

class ApplicationPage;
class Drawable;

#include "rwapi.h"

// External includes
#include <Windows.h>
#include <vector>

// Internal includes
#include <Common/Types.h>
#include <Common/rwmath.h>
#include <Elements/Pages/ApplicationPage.h>
#include <Elements/Drawables/Drawable.h>
#include <rwd2d.h>



class RWGUI_API Application
{
private:
	bool bShouldExit;
	HWND WindowHandler;
	HINSTANCE appInstance;
	RWD2D* Renderer;
	std::vector<ApplicationPage*> Pages;
	int activePage = 0;
	Drawable* currentHoveredDrawable = nullptr;
	Drawable* currentPressedDrawable = nullptr;
	bool Input[0x100] = { 0 };
public:

	int Run(HINSTANCE hInstance);
	void Stop();
	

	static LRESULT CALLBACK OnWindowProcStatic(HWND pWindowHandler, UINT uMsg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK OnWindowProc(HWND pWindowHandler, UINT uMsg, WPARAM wparam, LPARAM lparam);

	virtual void OnCommandLineArgument(char* arg,char* value) {};
	virtual void OnStop() {};
	virtual bool Update() { return false; };
	virtual void OnInit() {};
	virtual void OnDraw(RWD2D* d2d,ID2D1HwndRenderTarget* renderTarget);
	virtual void BuildPages(std::vector<ApplicationPage*>& outPages) {};
	virtual void OnKeyPressed(char key);
	virtual void OnKeyReleased(char key);


	void GlobalEvent(EGlobalEvent eventType);

	Drawable* GetDrawableAtPosition(Vector2D Position);
	Drawable* GetCurrentHoveredDrawable() { return currentHoveredDrawable; };

	void SetActivePage(int pageId);
	int GetActivePageID();
	ApplicationPage* GetActivePage();

	virtual char* GetApplicationName() = 0;
	virtual char* GetApplicationTitle();
	virtual Bounds GetDefaultWindowBounds();

	HWND GetWindowHandler();
	char* GetWindowClassName();
	Bounds GetCurrentWindowBounds();

	void InternalProcessCommandLine(char* cmd) {}; // TODO : Implement per arg on cmd argument function call OnCommandLineArgument
};

#define RUN_APPLICATION(AppClass)\
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,LPSTR cmd,int nCmdShow)\
{\
	Application* app = new AppClass();\
	app->InternalProcessCommandLine(cmd);\
	return app->Run(hInstance);\
};\

#endif