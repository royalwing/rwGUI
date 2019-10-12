#ifndef RWGUI_H
#define RWGUI_H

class ApplicationPage;
class Drawable;

#include "rwapi.h"

// External includes
#include <Windows.h>

// Internal includes
#include <Common/Types.h>
#include <Common/rwmath.h>
#include <Elements/Pages/ApplicationPage.h>
#include <Elements/Drawables/Drawable.h>
#include "Common/Containers.h"
#include <rwd2d.h>
#include <wincodec.h>
#include <chrono>



class RWGUI_API Application
{
private:
	bool bShouldExit;
	HWND WindowHandler;
	HINSTANCE appInstance;
	RWD2D* Renderer;
	std::vector<ApplicationPage*> Pages;
	int activePage = 0;
	int previousPage = -1;
	Drawable* currentHoveredDrawable = nullptr;
	Drawable* currentPressedDrawable = nullptr;
	Vector2D MinWindowSize;
	std::chrono::milliseconds lastUpdateTime;
	bool Input[0x100] = { 0 };
public:
	RWD2D* GetRenderer() const { return Renderer; };

	int Run(HINSTANCE hInstance);
	void Stop();
	

	static LRESULT CALLBACK OnWindowProcStatic(HWND pWindowHandler, UINT uMsg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK OnWindowProc(HWND pWindowHandler, UINT uMsg, WPARAM wparam, LPARAM lparam);

	virtual void OnCommandLineArgument(String arg,String value) {};
	virtual void OnStop() {};
	void InternalUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void OnInit() {};
	virtual void OnDraw(RWD2D* d2d,ID2D1HwndRenderTarget* renderTarget);
	virtual void BuildPages() {};
	void AddPage(ApplicationPage* appPage);
	virtual void OnKeyPressed(char key);
	virtual void OnKeyReleased(char key);


	void GlobalEvent(EGlobalEvent eventType);

	Drawable* GetDrawableAtPosition(Vector2D Position);
	Drawable* GetCurrentHoveredDrawable() { return currentHoveredDrawable; };

	void SetActivePage(int pageId);
	void NavigateBackward();
	int GetActivePageID();
	int GetPreviousPageID();
	ApplicationPage* GetActivePage();

	virtual String GetApplicationName() const = 0;
	virtual String GetApplicationTitle() const;
	virtual Bounds GetDefaultWindowBounds();

	HWND GetWindowHandler();
	HINSTANCE GetInstance() { return appInstance; };
	String GetWindowClassName() const;
	Bounds GetCurrentWindowBounds();

	void SetMinimalWindowSize(Vector2D minSize);
	Vector2D GetMinimalWindowSize();

	String GetApplicationFolder();

	void InternalProcessCommandLine(String cmd) {}; // TODO : Implement per arg on cmd argument function call OnCommandLineArgument
};

class ApplicationGetter
{
private:
	ApplicationGetter() {};
	~ApplicationGetter() {};
	Application* AppInstance = nullptr;
public:
	static ApplicationGetter* Get() { static ApplicationGetter instance; return &instance; };

	void SetApplication(Application* Instance)
	{
		AppInstance = Instance;
	};
	Application* GetApplication() {
		return AppInstance;
	};

};

#define GApplication ApplicationGetter::Get()->GetApplication()

#define RUN_APPLICATION(AppClass)\
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE prevInstance,LPSTR cmd,int nCmdShow)\
{\
	Application* app = new AppClass();\
	app->InternalProcessCommandLine(cmd);\
	return app->Run(hInstance);\
};\

#endif