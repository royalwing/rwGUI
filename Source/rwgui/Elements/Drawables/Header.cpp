#include "Header.h"


Header::Header(String name, String title)
	: Drawable(name)
{
	bInteractive = true;
	bIsNonClient = true;
	headerTitle = title;
	zOrder = -1;
}

void Header::Init()
{
	HeaderLayout = new Layout("HeaderLayout", ELayoutType::LayoutType_HORIZONTAL);
	HeaderLayout->bIsNonClient = true;
	HeaderLayout->SetContentPadding(8,0,8,0);
	HeaderLayout->OverrideHTResponse(HTCAPTION);
	AddChild(HeaderLayout);
	HeaderText = new TextLabel("HeaderText","");
	HeaderText->SetPadding(-4, 16, 0, 0);
	HeaderText->SetFontFamily("Lucida Sans Unicode");
	HeaderText->LayoutScale = 10.0f;
	HeaderLayout->AddChild(HeaderText);
}

void Header::Update(float DeltaTime)
{
	if(GetApplication()->GetPreviousPageID()>-1 && GetApplication()->GetActivePageID()>0 && BackButton == nullptr && HeaderLayout)
	{
		BackButton = new Button("HeaderBackButton",
		[](Application* app) {
			app->NavigateBackward();
		});
		BackButton->BackgroundAlignment = Button::EBackgroundAlignment::BA_StretchToFit;
		BackButton->SetBackgroundColor(Color(0, 0, 0, 0));
		BackButton->zOrder = -1;
		BackButton->SetBackgroundImage(IDB_BACKBTN);
		BackButton->SetSize(58.5f, 18);
		BackButton->SetFontSize(12);
		BackButton->SetCaption("Back");
		BackButton->bIgnoreLayoutScaling = true;
		HeaderLayout->AddChild(BackButton,true);
	}

	HeaderText->SetText(headerTitle);
	HeaderLayout->SetPosition(0, 0);
	HeaderLayout->SetSize(GetOuterBounds(IsNonClient()).Size.x, 35);
	SetPosition(0, 0);
	SetSize(GetOuterBounds(IsNonClient()).Size.x, 35);
	Drawable::Update(DeltaTime);
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

	Drawable::Draw(d2d, renderTarget);
}
