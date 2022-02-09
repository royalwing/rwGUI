#include "BasePage.h"

void BasePage::BuildPage()
{	

	background = new Background("Background", backgroundColor);
	background->SetBackgroundColor(backgroundColor);
	background->bIsNonClient = true;
	background->zOrder = -5;

	auto BorderVisibility = [App=GetApplication()]()->bool {
		return !App->IsWindowMaximized();
	};

	AddElement(background);
	AddElement(new Border("TopBorder", EBorderType::Top, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("BottomBorder", EBorderType::Bottom, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("LeftBorder", EBorderType::Left, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("RightBorder", EBorderType::Right, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("TopLeftBorder", EBorderType::TopLeft, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("TopRightBorder", EBorderType::TopRight, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("BottomRightBorder", EBorderType::BottomRight, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	AddElement(new Border("BottomLeftBorder", EBorderType::BottomLeft, borderColor, borderWidth))->SetVisilityLambda(BorderVisibility);
	header = new Header("Header", GetTitle());
	header->SetTextColor(Color(1.0f, 1.0f, 1.0f));

	class CloseButton : public Button
	{
		using Button::Button;
	public:
		virtual void Init()
		{
			SetBackgroundColor(Color(0, 0, 0, 0));
		};
		virtual void Update(float DeltaTime)
		{
			SetPosition(GetOuterBounds(IsNonClient()).Size.x - 18 - 12, 8);
			SetSize(18, 18);
			BackgroundAlignment = BA_StretchToFit;
		}
	};

	CloseButton* closeButton = new  CloseButton("CloseButton", [this](Button* Target) {
		GetApplication()->Stop();
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

void BasePage::SetTitle(String newtitle)
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

void BasePage::SetBackgroundColor(Color color)
{
	if(background)
		background->SetBackgroundColor(color);
	backgroundColor = color;
}

Color BasePage::GetBackgroundColor() const
{
	if(background)
		return background->GetBackgroundColor();
	return backgroundColor;
}

void BasePage::SetBorderColor(Color color)
{
	for(Drawable* el : Elements)
	{
		if(Border* brdr = dynamic_cast<Border*>(el))
		{
			brdr->borderColor = color;
		}
	}
	borderColor = color;
}

Color BasePage::GetBorderColor() const
{
	return borderColor;
}
