#include "BasePage.h"

void BasePage::BuildPage()
{	

	background = new Background("Background", backgroundColor);
	background->SetBackgroundColor(Color(0.3f, 0.3f, 0.3f, 1.0f));
	background->bIsNonClient = true;
	background->zOrder = -5;
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

	class CloseButton : public Button
	{
	public:
		CloseButton(char* name, OnButtonPressedDelegate btnPressed = nullptr) : Button(name, btnPressed) {};
		virtual void Update(float DeltaTime)
		{
			SetPosition(GetOuterBounds(IsNonClient()).Size.x - 18 - 12, 8);
			SetSize(18, 18);
			BackgroundAlignment = BA_StretchToFit;
		}
	};

	CloseButton* closeButton = new  CloseButton("CloseButton", [](Application* app) {
		app->Stop();
	});
	closeButton->bIsNonClient = true;
	closeButton->zOrder = -1;
	closeButton->SetBackgroundColor(Color(0.0f,0.0f,0.0f,0.0f));
	closeButton->SetBackgroundImage(IDB_CLOSEBTN);
	AddElement(closeButton);
	AddElement(header);

}

void BasePage::OnInit()
{
}

void BasePage::SetTitle(char * newtitle)
{
	ApplicationPage::SetTitle(newtitle);
	if (header) header->SetText(newtitle);
}


Bounds BasePage::GetClientBounds()
{
	Bounds bounds(0,0,0,0);
	bounds.Pos.y += 35;
	bounds.Size.y -= 35;
	return bounds;
}
