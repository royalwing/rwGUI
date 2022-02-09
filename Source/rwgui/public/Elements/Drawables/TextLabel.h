#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <Elements/Drawables/Drawable.h>
#include "Common/Containers.h"

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
	String Value;
	Color textColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
	String textFontFamily = "Arial";
	int textFontSize = 16;
	ETextStyle textFontStyle = TS_Regular;
	ETextVerticalAlignment textVerticalAlignment = TVA_TOP;
	ETextHorizontalAlignment textHorizontalAlignment = THA_LEFT;
public:
	TextLabel(String newname, String value);

	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	
	void SetText(String Text);
	String GetText();

	void SetTextColor(Color color);
	Color GetTextColor();

	void SetFontSize(int fontSize = 12);
	int GetFontSize();

	void SetFontFamily(String fontFamilyName = "Arial");
	String GetFontFamily();

	void SetFontStyle(ETextStyle textStyle);
	ETextStyle GetFontStyle();

	void SetVerticalAlignment(ETextVerticalAlignment Alignment) { textVerticalAlignment = Alignment; };
	void SetHorizontalAlignment(ETextHorizontalAlignment Alignment) { textHorizontalAlignment = Alignment; };
};

#endif // TEXTLABEL_H