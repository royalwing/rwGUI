#include "ResourceManager.h"
#include <rwgui.h>

ID2D1Brush * ResourceManager::MakeBrush(Color color)
{
	ID2D1SolidColorBrush* brush = nullptr;
	if (ResourceManager::Get()->renderTarget != nullptr)
	{
		for (auto resource : ResourceManager::Get()->Resources)
		{
			if (resource == nullptr) continue;
			Resource_SolidBrush* sBrush = dynamic_cast<Resource_SolidBrush*>(resource);
			if (sBrush!=nullptr && sBrush->clr == color)
			{
				return sBrush->GetValue();
			}
		}
		ResourceManager::Get()->renderTarget->CreateSolidColorBrush(color.ToD2D1ColorF(), &brush);
		new Resource_SolidBrush(color, brush);
	}
	return brush;
}

IDWriteTextFormat * ResourceManager::MakeTextFormat(char* aFontFamily, float fontSize,DWRITE_FONT_WEIGHT weight,DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	IDWriteTextFormat* format = nullptr;
	if (ResourceManager::Get()->writeFactory != nullptr)
	{
		ResourceManager* RM = ResourceManager::Get();
		std::wstring fontFamily(strlen(aFontFamily), L'#');
		mbstowcs(&fontFamily[0], aFontFamily, strlen(aFontFamily));
		for (auto resource : RM->Resources)
		{
			if (resource == nullptr) continue;
			Resource_TextFormat* sTextFormat = dynamic_cast<Resource_TextFormat*>(resource);
			if (sTextFormat != nullptr)
			{
				UINT32 familyNameLength = sTextFormat->value->GetFontFamilyNameLength();
				WCHAR* familyName = new WCHAR[familyNameLength+1];
				ZeroMemory(familyName, familyNameLength);
				if (sTextFormat->value->GetFontFamilyName(familyName, familyNameLength+1) == S_OK)
				{
					if (lstrcmpW(familyName, fontFamily.c_str()) == 0
						&& sTextFormat->value->GetFontSize() == fontSize
						&& sTextFormat->value->GetFontStretch() == stretch
						&& sTextFormat->value->GetFontStyle() == style
						&& sTextFormat->value->GetFontWeight() == weight
						)
					{
						delete[] familyName;
						return sTextFormat->GetValue();
					}
				}
				delete[] familyName;
			}
		}
		RM->writeFactory->CreateTextFormat(fontFamily.c_str(), nullptr, weight, style, stretch, fontSize, L"", &format);
		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
		new Resource_TextFormat(format);
	}
	return format;
}

ID2D1Bitmap * ResourceManager::MakeBitmap(char * bitmapPath)
{
	ID2D1Bitmap* bitmap = nullptr;
	if (ResourceManager::Get()->renderTarget != nullptr)
	{
		for (auto resource : ResourceManager::Get()->Resources)
		{
			if (resource == nullptr) continue;
			Resource_Bitmap* sBitmap = dynamic_cast<Resource_Bitmap*>(resource);
			if (sBitmap != nullptr && strcmp(sBitmap->bitmapPath,bitmapPath)==0)
			{
				return sBitmap->GetValue();
			}
		}
		if (ResourceManager::Get()->imageFactory!=nullptr)
		{
			IWICBitmapDecoder* pDecoder = nullptr;
			IWICBitmapFrameDecode* pSource = nullptr;
			IWICFormatConverter* pConverter = nullptr;


			std::wstring wBitmapPath(strlen(bitmapPath), L'#');
			mbstowcs(&wBitmapPath[0], bitmapPath, strlen(bitmapPath));

			HRESULT result = ResourceManager::Get()->imageFactory->CreateDecoderFromFilename(wBitmapPath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
			if (result == S_OK)
			{
				result = pDecoder->GetFrame(0, &pSource);
				if (result == S_OK)
				{
					result = ResourceManager::Get()->imageFactory->CreateFormatConverter(&pConverter);
					if (result == S_OK)
					{
						result = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
						if (result == S_OK)
						{
							result = ResourceManager::Get()->renderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, &bitmap);
							new Resource_Bitmap(bitmapPath, bitmap);
						}
					}
				}
			}
			if (pDecoder != nullptr) pDecoder->Release();
			if (pSource != nullptr) pSource->Release();
			if (pConverter != nullptr) pConverter->Release();
		}
	}
	return bitmap;
}

ID2D1Bitmap * ResourceManager::MakeBitmap(int ResourceID)
{
	ID2D1Bitmap* bitmap = nullptr;
	if (ResourceManager::Get()->renderTarget != nullptr)
	{
		for (auto resource : ResourceManager::Get()->Resources)
		{
			if (resource == nullptr) continue;
			Resource_Bitmap* sBitmap = dynamic_cast<Resource_Bitmap*>(resource);
			if (sBitmap != nullptr && sBitmap->ResourceID == ResourceID)
			{
				return sBitmap->GetValue();
			}
		}
		if (ResourceManager::Get()->imageFactory != nullptr)
		{
			IWICBitmapDecoder* pDecoder = nullptr;
			IWICBitmapFrameDecode* pSource = nullptr;
			IWICFormatConverter* pConverter = nullptr;

			HRSRC imageResourceHandle = nullptr;
			HGLOBAL imageResDataHandle = nullptr;
			void *pImageFile = nullptr;
			DWORD imageFileSize = 0;
			IWICStream* stream = nullptr;
			
			HMODULE pModule = GetModuleHandle("rwgui_x64.dll");

			imageResourceHandle = FindResource(pModule, MAKEINTRESOURCE(ResourceID), "PNG");
			HRESULT hr = (imageResourceHandle ? S_OK : E_FAIL);
			if (!SUCCEEDED(hr))
			{
				pModule = nullptr;
				imageResourceHandle = FindResource(pModule, MAKEINTRESOURCE(ResourceID), "PNG");
				hr = (imageResourceHandle ? S_OK : E_FAIL);
			}
			if (SUCCEEDED(hr))
			{
				imageResDataHandle = LoadResource(pModule, imageResourceHandle);
				hr = (imageResDataHandle ? S_OK : E_FAIL);
				if (SUCCEEDED(hr))
				{
					pImageFile = LockResource(imageResDataHandle);
					if (pImageFile)
					{
						imageFileSize = SizeofResource(pModule, imageResourceHandle);
						if (imageFileSize)
						{
							if (ResourceManager::Get()->imageFactory->CreateStream(&stream) == S_OK)
							{
								if (stream->InitializeFromMemory(reinterpret_cast<BYTE*>(pImageFile), imageFileSize));
							}
							else {
								stream = nullptr;
							}

						}
					}
				}
				if (stream != nullptr)
				{
					HRESULT result = ResourceManager::Get()->imageFactory->CreateDecoderFromStream(stream, nullptr, WICDecodeMetadataCacheOnLoad, &pDecoder);
					if (result == S_OK)
					{
						result = pDecoder->GetFrame(0, &pSource);
						if (result == S_OK)
						{
							result = ResourceManager::Get()->imageFactory->CreateFormatConverter(&pConverter);
							if (result == S_OK)
							{
								result = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
								if (result == S_OK)
								{
									result = ResourceManager::Get()->renderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, &bitmap);
									new Resource_Bitmap(ResourceID, bitmap);
								}
							}
						}
					}
					if (pDecoder != nullptr) pDecoder->Release();
					if (pSource != nullptr) pSource->Release();
					if (pConverter != nullptr) pConverter->Release();
				}
			}
		}
	}
	return bitmap;
}
