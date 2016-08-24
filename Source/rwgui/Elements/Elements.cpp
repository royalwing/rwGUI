#define DLLLIB
#include "../rwgui.h"
#include "Elements.h"

Drawable::Drawable(char * newname)
	: Outer(nullptr)
{
	name = newname;
	if (name == nullptr) name = "UnnamedElement";
}

void Drawable::AddChild(Drawable* child)
{
	if (child == nullptr) return;
	child->appPage = this->appPage;
	child->Outer = this;
	Elements.push_back(child);
}

void Drawable::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	for (Drawable* elem : Elements)
	{
		elem->Draw(d2d, renderTarget);
	}
}

ApplicationPage* Drawable::GetAppPage()
{
	return appPage;
}

Application* Drawable::GetApplication()
{
	return GetAppPage()->GetApplication();
}

Drawable * Drawable::GetOuter()
{
	return Outer;
}

char* Drawable::GetName()
{
	return name;
}

Bounds Drawable::GetBounds()
{
	return Bounds(0,0,0,0);
}

Bounds Drawable::GetOuterBounds()
{
	return GetOuter()!=nullptr ? GetOuter()->GetBounds() : appPage->GetBounds();
}

Bounds Drawable::GetAbsoluteBounds()
{
	Bounds result = GetBounds();
	Drawable* outer = GetOuter();
	while (outer != nullptr)
	{
		result += outer->GetBounds();
		outer = GetOuter();
	}
	result+=appPage->GetApplication()->GetCurrentWindowBounds();
	return result;
}

Drawable* Drawable::GetDrawableAtPosition(Vector2D Position)
{
	for (Drawable* element : Elements)
	{
		Bounds elBound = element->GetOuterBounds();
		elBound = elBound + element->GetSelectionBounds();
		if(element->IsInteractive() && elBound.IsInBound(Position)) return element->GetDrawableAtPosition(Position);
	}
	return this;
}

Border::Border(char* name,EBorderType type,Color color,int width)
	: Drawable(name)
	, borderType(type)
	, borderColor(color)
	, borderWidth(width)
{
	bInteractive = true;
}

void Border::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (borderBrush == nullptr) renderTarget->CreateSolidColorBrush(D2D1::ColorF(borderColor.r, borderColor.g, borderColor.b, borderColor.a), &borderBrush);
	renderTarget->FillRectangle(GetAbsoluteBounds().ToD2DRect(), borderBrush);
}

Bounds Border::GetBounds()
{
	Bounds outerBounds = GetOuterBounds();
	Bounds curBounds;
	switch (borderType)
	{
	case EBorderType::Top:
		curBounds.Pos.x = borderWidth;
		curBounds.Pos.y = 0;
		curBounds.Size.x = outerBounds.Size.x - borderWidth*2;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::Bottom:
		curBounds.Pos.x = borderWidth;
		curBounds.Pos.y = outerBounds.Size.y-borderWidth;
		curBounds.Size.x = outerBounds.Size.x - borderWidth*2;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::Left:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = outerBounds.Size.y - borderWidth*2;
		break;
	case EBorderType::Right:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth;
		curBounds.Pos.y = borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = outerBounds.Size.y - borderWidth * 2;
		break;
	case EBorderType::TopLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = 0;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::TopRight:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth;
		curBounds.Pos.y = 0;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::BottomRight:
		curBounds.Pos.x = outerBounds.Size.x-borderWidth;
		curBounds.Pos.y = outerBounds.Size.y-borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::BottomLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = outerBounds.Size.y-borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = borderWidth;
		break;
	}
	return curBounds;
}

Bounds Border::GetSelectionBounds()
{
	Bounds outerBounds = GetOuterBounds();
	Bounds curBounds;
	int selectionborder = 7;
	switch (borderType)
	{
	case EBorderType::Top:
		curBounds.Pos.x = borderWidth + selectionborder;
		curBounds.Pos.y = 0;
		curBounds.Size.x = outerBounds.Size.x - (borderWidth+selectionborder) * 2;
		curBounds.Size.y = borderWidth + selectionborder;
		break;
	case EBorderType::Bottom:
		curBounds.Pos.x = borderWidth + selectionborder;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth - selectionborder;
		curBounds.Size.x = outerBounds.Size.x - (borderWidth + selectionborder) * 2;
		curBounds.Size.y = borderWidth + selectionborder;
		break;
	case EBorderType::Left:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = borderWidth + selectionborder;
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = outerBounds.Size.y - (borderWidth + selectionborder) * 2;
		break;
	case EBorderType::Right:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth - selectionborder;
		curBounds.Pos.y = borderWidth + selectionborder;
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = outerBounds.Size.y - (borderWidth + selectionborder) * 2;
		break;
	case EBorderType::TopLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = 0;
		curBounds.Size.x = borderWidth+ selectionborder;
		curBounds.Size.y = borderWidth+ selectionborder;
		break;
	case EBorderType::TopRight:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth - selectionborder;
		curBounds.Pos.y = 0;
		curBounds.Size.x = borderWidth+ selectionborder;
		curBounds.Size.y = borderWidth+ selectionborder;
		break;
	case EBorderType::BottomRight:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth - selectionborder;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth - selectionborder;
		curBounds.Size.x = borderWidth+ selectionborder;
		curBounds.Size.y = borderWidth+ selectionborder;
		break;
	case EBorderType::BottomLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth - selectionborder;
		curBounds.Size.x = borderWidth+ selectionborder;
		curBounds.Size.y = borderWidth+ selectionborder;
		break;
	}
	return curBounds;
}
int Border::GetDrawableNCObjectType()
{
	switch (borderType)
	{
	case EBorderType::Bottom:
		return HTBOTTOM;
	case EBorderType::Top:
		return HTTOP;
	case EBorderType::Left:
		return HTLEFT;
	case EBorderType::Right:
		return HTRIGHT;
	case EBorderType::BottomRight:
		return HTBOTTOMRIGHT;
	case EBorderType::BottomLeft:
		return HTBOTTOMLEFT;
	case EBorderType::TopLeft:
		return HTTOPLEFT;
	case EBorderType::TopRight:
		return HTTOPRIGHT;
	}
	return HTNOWHERE;
};

Header::Header(char* name, char* title)
	: Drawable(name)
{
	bInteractive = true;
	headerTitle = title;
}

void Header::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (brush == nullptr)
	{
		D2D1_GRADIENT_STOP gradientStops[4];
		ID2D1GradientStopCollection* gradientStopCollection;
		gradientStops[0].color = Color(0.0,0.0,0.0).ToD2D1ColorF();
		gradientStops[0].position = 0.0f;
		gradientStops[1].color = Color(0.0, 0.0, 0.0).ToD2D1ColorF();
		gradientStops[1].position = 0.85f;
		gradientStops[2].color = Color(0.5f, 0.5f, 0.5f).ToD2D1ColorF();
		gradientStops[2].position = 0.86f;
		gradientStops[3].color = Color(0.9f, 0.9f, 0.9f).ToD2D1ColorF();
		gradientStops[3].position = 1.0f;
		renderTarget->CreateGradientStopCollection(gradientStops, 4, &gradientStopCollection);
		renderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0,0),D2D1::Point2F(0,GetBounds().Size.y)),gradientStopCollection,&brush);
	}
	if (textBrush == nullptr)
	{
		renderTarget->CreateSolidColorBrush(Color(0.0f, 0.0f, 0.0f).ToD2D1ColorF(), &textBrush);
	}
	if (textFormat == nullptr)
	{
		d2d->GetWriteFactory()->CreateTextFormat(L"Trajan Pro", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 14, L"", &textFormat);
	}
	//renderTarget->FillRectangle(GetBounds().ToD2DRect(), brush);
	std::wstring titleText(strlen(headerTitle),L'#');
	mbstowcs(&titleText[0], headerTitle, strlen(headerTitle));
	renderTarget->DrawTextA(titleText.c_str(), (UINT32)strlen(headerTitle), textFormat, (GetBounds() + Bounds(15, 8, 0, 0)).ToD2DRect(), textBrush);
}

Bounds Header::GetBounds()
{
	Bounds OuterBounds = GetOuterBounds();
	Bounds result;
	result.Pos = Vector2D(0,0);
	result.Size = OuterBounds.Size;
	result.Size.y = 35;
	return result;
}


Background::Background(char* name, Color inColor)
	: Drawable(name)
	, color(inColor)
{
	bInteractive = false;
}

void Background::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	if (brush == nullptr) renderTarget->CreateSolidColorBrush(D2D1::ColorF(color.r, color.g, color.b, color.a), &brush);
	renderTarget->FillRectangle(GetAbsoluteBounds().ToD2DRect(), brush);
}

Button::Button(char* name,Bounds bounds, Button::OnButtonPressedDelegate OnPress)
	: Drawable(name)
{
	bInteractive = true;
	OnButtonPressed = OnPress;
	Position = bounds.Pos;
	Size = bounds.Size;
}

void Button::OnMouseClick()
{
	if (OnButtonPressed != nullptr) OnButtonPressed(GetApplication());
}

void Button::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{

	Drawable* curHovered = GetApplication()->GetCurrentHoveredDrawable();
	if(curHovered !=nullptr &&  curHovered == this)
	{
		
	}
}

Bounds Button::GetBounds()
{
	return Bounds(Position,Size);
}

ApplicationPage::ApplicationPage()
{
}

ApplicationPage::~ApplicationPage()
{
}

void ApplicationPage::AddElement(Drawable * inElement)
{
	if (inElement == nullptr) return;
	inElement->appPage = this;
	inElement->Outer = nullptr;
	Elements.push_back(inElement);
}

Bounds ApplicationPage::GetBounds()
{
	return App->GetCurrentWindowBounds();
}

Application* ApplicationPage::GetApplication()
{
	return App;
}

Drawable * ApplicationPage::GetDrawableAtPosition(Vector2D Position)
{
	for (int i = 0; i < Elements.size(); i++)
	{
		Bounds outerElem = Elements[i]->GetOuterBounds();
		Bounds elemBounds = Elements[i]->GetSelectionBounds();
		elemBounds.Pos.x += outerElem.Pos.x;
		elemBounds.Pos.y += outerElem.Pos.y;
		if (Elements[i]->IsInteractive()
			&& elemBounds.IsInBound(Position))
		{
			return Elements[i]->GetDrawableAtPosition(Position);
		}
	}
	return nullptr;
}

void ApplicationPage::Draw(RWD2D * d2d, ID2D1HwndRenderTarget * renderTarget)
{
	for (Drawable* elem : Elements)
	{
		elem->Draw(d2d,renderTarget);
	}
}

BasePage::BasePage(char* pageTitle,Color backgroundColor, Color borderColor, int borderWidth)
{
	AddElement(new Background("Background", backgroundColor));
	AddElement(new Border("TopBorder", EBorderType::Top, borderColor, borderWidth));
	AddElement(new Border("BottomBorder", EBorderType::Bottom, borderColor, borderWidth));
	AddElement(new Border("LeftBorder", EBorderType::Left, borderColor, borderWidth));
	AddElement(new Border("RightBorder", EBorderType::Right, borderColor, borderWidth));
	AddElement(new Border("TopLeftBorder", EBorderType::TopLeft, borderColor, borderWidth));
	AddElement(new Border("TopRightBorder", EBorderType::TopRight, borderColor, borderWidth));
	AddElement(new Border("BottomRightBorder", EBorderType::BottomRight, borderColor, borderWidth));
	AddElement(new Border("BottomLeftBorder", EBorderType::BottomLeft, borderColor, borderWidth));
	AddElement(new Button("CloseButton", Bounds(0,0,32,32), [](Application* app) {
		app->Stop();
	}));
	AddElement(new Header("Header", pageTitle));

}