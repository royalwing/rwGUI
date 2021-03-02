#ifndef HEADER_H
#define HEADER_H


#include <Elements/Drawables/Drawable.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/TextLabel.h>
#include <Elements/Drawables/Layout.h>

typedef void (*BackButtonDelegate)(Application* app);

class RWGUI_API Header : public Drawable
{
private:
	ID2D1LinearGradientBrush* brush = nullptr;
	ID2D1SolidColorBrush* textBrush = nullptr;
	IDWriteTextFormat* textFormat = nullptr;
	Color textColor;
	Layout* HeaderLayout = nullptr;
	Button* BackButton = nullptr;
	TextLabel* HeaderText = nullptr;
	String headerTitle = "";
public:
	Header(String name, String title);
	virtual void Init();
	virtual void Update(float DeltaTime) override;
	virtual void Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget) override;
	virtual int GetDrawableNCObjectType() { return HTCAPTION; };
	void SetTextColor(Color tColor) { textColor = tColor; if (textBrush != nullptr) { textBrush->Release(); textBrush = nullptr; } };
	void SetText(String newTitle) { headerTitle = newTitle; };

};



#endif