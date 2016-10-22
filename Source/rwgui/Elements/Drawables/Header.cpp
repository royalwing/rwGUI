#include "Header.h"


Header::Header(char* name, char* title)
	: Drawable(name)
{
	bInteractive = true;
	bIsNonClient = true;
	headerTitle = title;
	zOrder = -1;
}

void Header::Update(float DeltaTime)
{
	SetPosition(0, 0);
	SetSize(GetOuterBounds(IsNonClient()).Size.x, 35);
}

void Header::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (brush == nullptr)
	{
		D2D1_GRADIENT_STOP gradientStops[4];
		ID2D1GradientStopCollection* gradientStopCollection;
		gradientStops[0].color = Color(0.0, 0.0, 0.0).ToD2D1ColorF();
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = Color(0.0, 0.0, 0.0).ToD2D1ColorF();
		gradientStops[1].position = 0.85f;
		gradientStops[2].color = Color(0.5f, 0.5f, 0.5f).ToD2D1ColorF();
		gradientStops[2].position = 0.86f;
		gradientStops[3].color = Color(0.9f, 0.9f, 0.9f).ToD2D1ColorF();
		gradientStops[3].position = 1.0f;
		renderTarget->CreateGradientStopCollection(gradientStops, 4, &gradientStopCollection);
		renderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(0, GetBounds().Size.y)), gradientStopCollection, &brush);
	}
	if (textBrush == nullptr)
	{
		renderTarget->CreateSolidColorBrush(textColor.ToD2D1ColorF(), &textBrush);
	}
	if (textFormat == nullptr)
	{
		d2d->GetWriteFactory()->CreateTextFormat(L"Trajan Pro", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14, L"", &textFormat);
	}
	//renderTarget->FillRectangle(GetBounds().ToD2DRect(), brush);
	std::wstring titleText(strlen(headerTitle), L'#');
	mbstowcs(&titleText[0], headerTitle, strlen(headerTitle));
	renderTarget->DrawTextA(titleText.c_str(), (UINT32)strlen(headerTitle), textFormat, (GetBounds() + Bounds(15, 8, 0, 0)).ToD2DRect(), textBrush);
}
