#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <Elements/Drawables/Drawable.h>

enum ETextStyle
{
	TS_Regular = 0,
	TS_Bold,
	TS_Italic
};

class RWGUI_API TextLabel : public Drawable
{
private:
	wchar_t* Value = L"";
	Color textColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	char* textFontFamily = "Arial";
	int textFontSize = 16;
	ETextStyle textFontStyle = TS_Regular;
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
};

#endif // TEXTLABEL_H