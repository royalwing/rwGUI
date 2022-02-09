#ifndef RWD2D_H
#define RWD2D_H

#include "rwapi.h"
#pragma comment(lib,"d2d1.lib")
#include <d2d1.h>
#include <dwrite.h>
#include<Common/rwmath.h>
#include <wincodec.h>

class Application;


class RWD2D
{
	IWICImagingFactory* imagefactory;
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* renderTarget;
	IDWriteFactory* writeFactory;
	Application* application;
	bool bInitialized;
public:

	
	RWD2D() : imagefactory(nullptr), factory(nullptr), renderTarget(nullptr) { internal_ClearColor = Color(0, 0, 0).ToD2D1ColorF(); };
	~RWD2D() {
		if (imagefactory) imagefactory->Release();
		if (factory) factory->Release();
		if (renderTarget) renderTarget->Release();
	};

	bool Init(Application* app);
	void BeginDraw() { renderTarget->BeginDraw(); };
	void EndDraw() { renderTarget->EndDraw(); };
	void Resize(int x, int y);

	IWICImagingFactory* GetWICFactory() { return imagefactory; };
	ID2D1Factory* GetFactory() { return factory; };
	IDWriteFactory* GetWriteFactory() { return writeFactory; };
	ID2D1HwndRenderTarget* GetRenderTarget() { return renderTarget; };

	bool Update();
private:
	D2D1_COLOR_F internal_ClearColor;
};

#endif