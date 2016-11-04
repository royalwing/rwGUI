#include "TextLabel.h"

TextLabel::TextLabel(char * newname, char * value)
	: Drawable(newname)
{
	bInteractive = false;
	SetText(value);
}

void TextLabel::Draw(RWD2D * d2d, ID2D1HwndRenderTarget * renderTarget)
{
	DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_REGULAR;
	DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL;
	if (textFontStyle == TS_Bold) weight = DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD;
	if (textFontStyle == TS_Italic) style = DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC;

	IDWriteTextFormat* textFormat = MAKETEXTFORMAT(textFontFamily, textFontSize, weight, style, DWRITE_FONT_STRETCH_EXTRA_EXPANDED);
	IDWriteTextLayout* textLayout = nullptr;
	HRESULT result = d2d->GetWriteFactory()->CreateTextLayout(Value, wcslen(Value), textFormat, GetBounds().Size.x, GetBounds().Size.y, &textLayout);
	if (result != S_OK) return;
	DWRITE_TEXT_METRICS metrics;
	textLayout->GetMetrics(&metrics);
	
	Bounds drawBounds = GetBounds();
	switch (textVerticalAlignment)
	{
	case ETextVerticalAlignment::TVA_MIDDLE:
		drawBounds.Pos.y += drawBounds.Size.y / 2 - metrics.height / 2;
		break;
	case ETextVerticalAlignment::TVA_BOTTOM:
		drawBounds.Pos.y += drawBounds.Size.y - metrics.height;
		break;
	}

	switch (textHorizontalAlignment)
	{
	case ETextHorizontalAlignment::THA_MIDDLE:
		drawBounds.Pos.x += drawBounds.Size.x / 2 - metrics.width / 2;
		break;
	case ETextHorizontalAlignment::THA_RIGHT:
		drawBounds.Pos.x += drawBounds.Size.x - metrics.width;
		break;
	}

	renderTarget->DrawText(Value, wcslen(Value), textFormat, drawBounds.ToD2DRect(), MAKEBRUSH(textColor));

}

void TextLabel::SetText(char * Text)
{
	int captionLen = strlen(Text);
	Value = new wchar_t[captionLen];
	Value[captionLen] = '\0';
	mbstowcs(Value, Text, strlen(Text));
}

char * TextLabel::GetText()
{
	int captionLen = wcslen(Value);
	char* result = new char[captionLen];
	wcstombs(result, Value, captionLen);
	return result;
}

void TextLabel::SetTextColor(Color color)
{
	textColor = color;
}

Color TextLabel::GetTextColor()
{
	return textColor;
}

void TextLabel::SetFontSize(int fontSize)
{
	textFontSize = fontSize;
	if (textFontSize < 8) textFontSize = 8;
}

int TextLabel::GetFontSize()
{
	return textFontSize;
}

void TextLabel::SetFontFamily(char * fontFamilyName)
{
	textFontFamily = fontFamilyName;
}

char * TextLabel::GetFontFamily()
{
	return textFontFamily;
}

void TextLabel::SetFontStyle(ETextStyle textStyle)
{
	textFontStyle = textStyle;
}

ETextStyle TextLabel::GetFontStyle()
{
	return textFontStyle;
}
