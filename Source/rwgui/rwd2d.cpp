#include "rwd2d.h"
#include "rwgui.h"
#include <Resources/ResourceManager.h>

bool RWD2D::Init(Application * app)
{
	application = app;
	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (result != S_OK) return false;

	RECT rect;
	GetClientRect(application->GetWindowHandler(), &rect);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	props.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;

	result = factory->CreateHwndRenderTarget(
		props,
		D2D1::HwndRenderTargetProperties(application->GetWindowHandler(), D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
		);
	if (result != S_OK) return false;
	ResourceManager::Get()->SetReferenceRenderTarget(renderTarget);
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if (result != S_OK) return false;
	ResourceManager::Get()->SetReferenceDirectWriteFactory(writeFactory);
	result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&imageFactory);
	if (result != S_OK) return false;
	ResourceManager::Get()->SetReferenceImageFactory(imageFactory);
	bInitialized = true;
	return true;
}

void RWD2D::Resize(int x,int y)
{
	if(renderTarget) renderTarget->Resize(D2D1::SizeU(x, y));
}

bool RWD2D::Update()
{
	if (!bInitialized) return true;
	BeginDraw();
	renderTarget->Clear(D2D1::ColorF(0,0,0));
	if (application) application->OnDraw(this,renderTarget);
	EndDraw();
	return true;
}
