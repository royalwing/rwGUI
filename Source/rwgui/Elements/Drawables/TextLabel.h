#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <Elements/Drawables/Drawable.h>

enum ETextStyle
{
	TS_Regular = 0,
	TS_Bold,
	TS_Italic
};

enum ETextHorizontalAlignment
{
	THA_LEFT = 0,
	THA_MIDDLE,
	THA_RIGHT
};

enum ETextVerticalAlignment
{
	TVA_TOP = 0,
	TVA_MIDDLE,
	TVA_BOTTOM
};

class RWGUI_API TextLabel : public Drawable
{
private:
	wchar_t* Value = L"";
	Color textColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	char* textFontFamily = "Arial";
	int textFontSize = 16;
	ETextStyle textFontStyle = TS_Regular;
	ETextVerticalAlignment textVerticalAlignment = TVA_TOP;
	ETextHorizontalAlignment textHorizontalAlignment = THA_LEFT;
public:
	TextLabel(char* newname, char* value);

	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	
	void SetText(char* Text);
	char* GetText();

	void SetTextColor(Color color);
	Color GetTextColor();

	void SetFontSize(int fontSize = 12);
	int GetFontSize();

	void SetFontFamily(char* fontFamilyName = "Arial");
	char* GetFontFamily();

	void SetFontStyle(ETextStyle textStyle);
	ETextStyle GetFontStyle();

	void SetVerticalAlignment(ETextVerticalAlignment Alignment) { textVerticalAlignment = Alignment; };
	void SetHorizontalAlignment(ETextHorizontalAlignment Alignment) { textHorizontalAlignment = Alignment; };
};

#endif // TEXTLABEL_H