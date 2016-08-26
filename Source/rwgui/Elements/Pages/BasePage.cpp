#include "BasePage.h"
#include <Elements/Drawables/Background.h>
#include <Elements/Drawables/Border.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/Header.h>

void BasePage::BuildPage()
{	

	Background* background = new Background("Background", backgroundColor);
	background->SetBackgroundImage(backgroundPath);
	AddElement(background);
	AddElement(new Border("TopBorder", EBorderType::Top, borderColor, borderWidth));
	AddElement(new Border("BottomBorder", EBorderType::Bottom, borderColor, borderWidth));
	AddElement(new Border("LeftBorder", EBorderType::Left, borderColor, borderWidth));
	AddElement(new Border("RightBorder", EBorderType::Right, borderColor, borderWidth));
	AddElement(new Border("TopLeftBorder", EBorderType::TopLeft, borderColor, borderWidth));
	AddElement(new Border("TopRightBorder", EBorderType::TopRight, borderColor, borderWidth));
	AddElement(new Border("BottomRightBorder", EBorderType::BottomRight, borderColor, borderWidth));
	AddElement(new Border("BottomLeftBorder", EBorderType::BottomLeft, borderColor, borderWidth));
	header = new Header("Header", GetTitle());
	header->SetTextColor(Color(1.0f, 1.0f, 1.0f));
	Bounds pageBounds = GetBounds();

	class CloseButton : public Button
	{
	public:
		CloseButton(char* name, OnButtonPressedDelegate btnPressed = nullptr) : Button(name, btnPressed) {};
		virtual void Update(float DeltaTime)
		{
			SetPosition(GetOuterBounds().Size.x - 18 - 12, 8);
			SetSize(18, 18);
		}
	};

	CloseButton* closeButton = new  CloseButton("CloseButton", [](Application* app) {
		app->Stop();
	});
	closeButton->SetBackgroundColor(Color(0.0f,0.0f,0.0f,0.0f));
	closeButton->SetBackgroundImage(closeBtnPath);
	AddElement(closeButton);
	AddElement(header);
}

void BasePage::SetTitle(char * newtitle)
{
	ApplicationPage::SetTitle(newtitle);
	if (header) header->SetText(newtitle);
}
