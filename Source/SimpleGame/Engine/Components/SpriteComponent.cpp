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
	IWICFormatConverter* WicFormatConverter;
	WicFactory->CreateFormatConverter(&WicFormatConverter);
	IWICBitmapDecoder* pDecoder = nullptr;
	if(SUCCEEDED(WicFactory->CreateDecoderFromFilename(newSpriteSource.ToWideString(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder)))
	{
		IWICBitmapFrameDecode* pFrameDecode = nullptr;
		pDecoder->GetFrame(0, &pFrameDecode);
		WicFormatConverter->Initialize(pFrameDecode, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
		if(!SUCCEEDED(GApplication->GetRenderer()->GetRenderTarget()->CreateBitmapFromWicBitmap(WicFormatConverter, &SpriteBitmap)))
		{
			RW_WARNING("Failed to create bitmap for :");
			RW_WARNING(newSpriteSource);
		}
		lastSpriteSource = newSpriteSource;
		pDecoder->Release();
		WicFormatConverter->Release();
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
		Bounds bounds = Bounds(-Size.x*0.5f, -Size.y*0.5f, Size.x, Size.y);
		renderTarget->DrawBitmap(SpriteBitmap, bounds.ToD2DRect(), GetSpriteOpacity());
	}
}
