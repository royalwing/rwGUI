#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>
#include <Common/rwmath.h>

class ResourceManager;

class ResourceBase
{
public:
	ResourceBase() {};
	~ResourceBase() { Release(); };
	virtual void Release() {};
};

template <typename T>
class Resource : public ResourceBase
{
public:
	T* value;
	Resource(T* nVal) { value = nVal; ResourceManager::Get()->RegisterResource(this); };
	~Resource() { HCURSOR cursor; };
	T* GetValue() { return value; };
};

class Resource_Cursor : public Resource<HCURSOR>
{
};

class Resource_SolidBrush : public Resource<ID2D1Brush>
{
public:
	Color clr;
	Resource_SolidBrush(Color color, ID2D1Brush* nVal) : Resource(nVal) { clr = color;};

	virtual void Release() override { if(value!=nullptr) value->Release(); };
};

class Resource_TextFormat : public Resource<IDWriteTextFormat>
{
public:
	Resource_TextFormat(IDWriteTextFormat* nVal) : Resource(nVal) {}
	virtual void Release() override { if (value != nullptr) value->Release(); };
};

class Resource_Bitmap : public Resource<ID2D1Bitmap>
{
public:
	char* bitmapPath;
	Resource_Bitmap(char* bitmapPath, ID2D1Bitmap* nVal) : Resource(nVal) {};
	virtual void Release() override { if (value != nullptr) value->Release(); };

};

class ResourceManager
{
private:
	ResourceManager() {};
	~ResourceManager() {};

	ID2D1RenderTarget* renderTarget = nullptr;
	IDWriteFactory* writeFactory = nullptr;
	IWICImagingFactory* imageFactory = nullptr;
public:
	std::vector<ResourceBase*> Resources;

	static ResourceManager* Get() { static ResourceManager instance; return &instance; };

	void SetReferenceRenderTarget(ID2D1RenderTarget* rt) { renderTarget = rt; };
	void SetReferenceDirectWriteFactory(IDWriteFactory* factory) { writeFactory = factory; };
	void SetReferenceImageFactory(IWICImagingFactory* imgfactory) { imageFactory = imgfactory; };

	void RegisterResource(ResourceBase* resource) { Resources.push_back(resource); };

	static ID2D1Brush* MakeBrush(Color color);
	static IDWriteTextFormat* MakeTextFormat(char* aFontFamily, float fontSize, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);
	static ID2D1Bitmap* MakeBitmap(char* bitmapPath);
};

#define MAKEBRUSH ResourceManager::MakeBrush
#define MAKETEXTFORMAT ResourceManager::MakeTextFormat
#define MAKEBITMAP ResourceManager::MakeBitmap

#endif