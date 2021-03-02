#ifndef RWGUI_H
#define RWGUI_H
#include "Common/DebugHelpers.h"

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
	std::chrono::milliseconds startTime;
	bool Input[0x100] = { 0 };
	Vector2D LocalMousePosition;
public:
	Application();

	RWD2D* GetRenderer() const { return Renderer; }
	float GetRealTimeSeconds() const;;

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
	virtual void OnMouseMove(const Vector2D& PrevPosition, const Vector2D& Position);
	virtual void OnMouseWheel(float Delta);
	virtual void OnKeyPressed(char key);
	virtual void OnKeyReleased(char key);
	virtual void OnKeyStateChanged(char key, bool bPressed) {};

	bool IsKeyDown(char key) const;

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

	void SetWindowSize(Vector2D windowSize);
	virtual void OnResize(Vector2D inSize);

	void Maximize();
	bool IsWindowMaximized() const;

	void SetLocalMousePosition(const Vector2D& MousePosition);
	Vector2D GetLocalMousePosition() const;

	String GetApplicationFolder();

	void InternalProcessCommandLine(String cmd) {}; // TODO : Implement per arg on cmd argument function call OnCommandLineArgument
};

class RWGUI_API ApplicationGetter
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

// Container that holds various D2D resources till it will be destroyed and that releases it correctly
template<typename ResourceType>
struct rwD2DResourceContainer
{
public:
	rwD2DResourceContainer() {
		ResourcePtr = nullptr;
	};

	~rwD2DResourceContainer()
	{
		if (ResourcePtr != nullptr)
		{
			ResourcePtr->Release();
			ResourcePtr = nullptr;
		}
	}

	rwD2DResourceContainer& operator=(const ResourceType* Other)
	{
		if (ResourcePtr != Other)
		{
			Reset();
		}
		ResourcePtr = Other;
		return *this;
	}

	operator ResourceType* () { return ResourcePtr; };
	operator ResourceType** () { return &ResourcePtr; };

	bool isSet() const { return ResourcePtr != nullptr; };
	void Reset() { if (isSet()) { ResourcePtr->Release(); ResourcePtr = nullptr; } };
private:
	ResourceType* ResourcePtr = nullptr;
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