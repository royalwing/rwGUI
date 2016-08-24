#include "ApplicationPage.h"


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
		elem->Draw(d2d, renderTarget);
	}
}