#include "SpriteComponent.h"
#include "rwgui.h"
#include "Common/DebugHelpers.h"

SpriteComponent::SpriteComponent(String Name, Entity* Owner, String SpriteSource)
	: VisualComponent(Name, Owner)
{
	SetSpriteBitmap(SpriteSource);
}

void SpriteComponent::SetSpriteBitmap(String newSpriteSource)
{
	if (lastSpriteSource == newSpriteSource) return;
	if(SpriteBitmap!=nullptr)
	{
		SpriteBitmap->Release();
		SpriteBitmap = nullptr;
	}
	IWICImagingFactory* WicFactory = ApplicationGetter::Get()->GetApplication()->GetRenderer()->GetWICFactory();
	IWICFormatConverter* WicFormatConverter = ApplicationGetter::Get()->GetApplication()->GetRenderer()->GetFormatConverter();
	IWICBitmapDecoder* pDecoder = nullptr;
	if(SUCCEEDED(WicFactory->CreateDecoderFromFilename(newSpriteSource.ToWideString(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder)))
	{
		IWICBitmapFrameDecode* pFrameDecode = nullptr;
		pDecoder->GetFrame(0, &pFrameDecode);
		WicFormatConverter->Initialize(pFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
		GApplication->GetRenderer()->GetRenderTarget()->CreateBitmapFromWicBitmap(WicFormatConverter, &SpriteBitmap);
		lastSpriteSource = newSpriteSource;
		pDecoder->Release();
		return;
	} else
	{
		RW_WARNING("Failed to create bitmap decoder for bitmap :");
		RW_WARNING(newSpriteSource);
	}
	lastSpriteSource = "";
}

void SpriteComponent::Draw(ID2D1BitmapRenderTarget* renderTarget)
{
	if(SpriteBitmap)
	{
		renderTarget->DrawBitmap(SpriteBitmap, Bounds(0,0, 512, 512).ToD2DRect());
	}
}
