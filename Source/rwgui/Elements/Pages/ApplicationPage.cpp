#include "ApplicationPage.h"
#include <algorithm>

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

Bounds ApplicationPage::GetBounds(bool bNonClient)
{
	if (bNonClient)
	{
		return App->GetCurrentWindowBounds();
	}
	Bounds ClientBounds = App->GetCurrentWindowBounds();
	ClientBounds = ClientBounds + GetClientBounds();
	return ClientBounds;
}

Application* ApplicationPage::GetApplication()
{
	return App;
}

Drawable * ApplicationPage::GetDrawableAtPosition(Vector2D Position)
{
	for (int i = 0; i < Elements.size(); i++)
	{
		Bounds outerElem = Elements[i]->GetOuterBounds(Elements[i]->IsNonClient());
		Bounds elemBounds = Elements[i]->GetSelectionBounds();
		//elemBounds.Pos.x += outerElem.Pos.x;
		//elemBounds.Pos.y += outerElem.Pos.y;
		if (Elements[i]->IsInteractive()
			&& elemBounds.IsInBound(Position))
		{
			return Elements[i]->GetDrawableAtPosition(Position);
		}
	}
	return nullptr;
}

void ApplicationPage::Init()
{
	OnInit();
	for (auto e : Elements)
	{
		e->InternalInit();
	}
}

void ApplicationPage::Draw(RWD2D * d2d, ID2D1HwndRenderTarget * renderTarget)
{
	for (Drawable* elem : Elements)
	{
		elem->Draw(d2d, renderTarget);
	}
}


int sort(Drawable* a, Drawable* b) { return a->zOrder < b->zOrder ? 1 : 0; };

void ApplicationPage::InternalUpdate(float DeltaTime)
{
	std::sort(std::begin(Elements), std::end(Elements),sort);
	for (Drawable* element : Elements)
	{
		element->InternalUpdate(DeltaTime);
	}
	Update(DeltaTime);
}

void ApplicationPage::Update(float DeltaTime)
{
}


void ApplicationPage::SetApp(Application* nApp)
{
	this->App = nApp;
}



void ApplicationPage::SetTitle(String newtitle)
{
	pageTitle = newtitle;
}

String ApplicationPage::GetTitle()
{
	return pageTitle;
}


