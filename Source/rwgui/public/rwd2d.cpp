#include "rwd2d.h"
#include "rwgui.h"
#include <Resources/ResourceManager.h>
#include <Common/DebugHelpers.h>

bool RWD2D::Init(Application * app)
{
	application = app;
	ResourceManager::Get()->SetApplication(app);


	HRESULT result = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
	if (result != S_OK)
	{
		RW_ERROR("Failed to create D2D1Factory.");
		return false;
	}

	RECT rect;
	GetClientRect(application->GetWindowHandler(), &rect);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
	props.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;

	result = factory->CreateHwndRenderTarget(
		props,
		D2D1::HwndRenderTargetProperties(application->GetWindowHandler(), D2D1::SizeU(rect.right, rect.bottom)),
		&renderTarget
		);
	if (result != S_OK)
	{
		RW_ERROR("Failed to create Render Target.");
		return false;
	}
	ResourceManager::Get()->SetReferenceRenderTarget(renderTarget);
	result = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&writeFactory));
	if (result != S_OK)
	{
		RW_ERROR("Failed to create DWriteFactory.");
		return false;
	}
	ResourceManager::Get()->SetReferenceDirectWriteFactory(writeFactory);
	result = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&imagefactory);
	if (result != S_OK)
	{
		switch (result)
		{
		case E_POINTER:
			RW_ERROR("Error creating ImageFactory. The ppv paramater is NULL.");
			break;
		case E_NOINTERFACE:
			RW_ERROR("Error creating ImageFactory. Specified class does not implement the requested interface.");
			break;
		case CLASS_E_NOAGGREGATION:
			RW_ERROR("Error creating ImageFactory. This class can not be created as agregate.");
			break;
		case REGDB_E_CLASSNOTREG:
			RW_ERROR("Error creating ImageFactory. Specified class is not registered.");
			break;
		default:
			RW_ERROR("Error creating ImageFactory.");
			break;
		}
		return false;
	}


	ResourceManager::Get()->SetReferenceImageFactory(imagefactory);
	bInitialized = true;
	return true;
}

void RWD2D::Resize(int x,int y)
{
	if (renderTarget)
	{
		HRESULT result = renderTarget->Resize(D2D1::SizeU(x, y));
		if (result != S_OK)
		{
			RW_WARNING("Failed to resize render target.");
		}
	}
}

bool RWD2D::Update()
{
	if (!bInitialized) return true;
	BeginDraw();
	renderTarget->Clear(internal_ClearColor);
	if (application!=nullptr)
		application->OnDraw(this,renderTarget);
	EndDraw();
	return true;
}
