#ifndef HEADER_H
#define HEADER_H

#include <Elements/Drawables/Drawable.h>

class RWGUI_API Header : public Drawable
{
private:
	ID2D1LinearGradientBrush* brush = nullptr;
	ID2D1SolidColorBrush* textBrush = nullptr;
	IDWriteTextFormat* textFormat = nullptr;
	Color textColor;
	char* headerTitle = "";
public:
	Header(char* name, char* title);
	virtual void Update(float DeltaTime) override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual int GetDrawableNCObjectType() { return HTCAPTION; };
	void SetTextColor(Color tColor) { textColor = tColor; if (textBrush != nullptr) { textBrush->Release(); textBrush = nullptr; } };
	void SetText(char* newTitle) { headerTitle = newTitle; };
};



#endif