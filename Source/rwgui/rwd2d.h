#ifndef RWD2D_H
#define RWD2D_H

#include "rwapi.h"
#pragma comment(lib,"d2d1.lib")
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

class Application;


class RWD2D
{
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* writeFactory;
	IWICImagingFactory* imageFactory;
	Application* application;
	bool bInitialized;
public:


	RWD2D() : factory(nullptr),renderTarget(nullptr) { };
	~RWD2D() {
		if (factory) factory->Release();
		if (renderTarget) renderTarget->Release();
	};

	bool Init(Application* app);
	void BeginDraw() { renderTarget->BeginDraw(); };
	void EndDraw() { renderTarget->EndDraw(); };
	void Resize();


	ID2D1Factory* GetFactory() { return factory; };
	IDWriteFactory* GetWriteFactory() { return writeFactory; };
	ID2D1HwndRenderTarget* GetRenderTarget() { return renderTarget; };

	bool Update();
};

#endif