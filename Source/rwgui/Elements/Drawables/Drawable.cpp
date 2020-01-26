#include "Drawable.h"


Drawable::Drawable(String newname)
	: Outer(nullptr)
{
	Padding.left = 0;
	Padding.right = 0;
	Padding.bottom = 0;
	Padding.top = 0;
	bIsNonClient = false;
	name = newname;
	if (name.IsEmpty()) name = "UnnamedElement";
}

void Drawable::AddChild(Drawable* child, bool bAddToBeginning)
{
	if (child == nullptr) return;
	child->appPage = this->appPage;
	child->Outer = this;
	if (bAddToBeginning)
		Elements.insert(Elements.begin(), child);
	else
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

void Drawable::InternalInit()
{
	Init();
	for (auto e : Elements) e->InternalInit();
}


void Drawable::OnWindowResize(const Vector2D& inSize)
{
	for (Drawable* Element : Elements)
		Element->OnWindowResize(inSize);
}

void Drawable::InternalUpdate(float DeltaTime)
{
	for (int i = 0; i < Elements.size(); i++)
	{
		if (Elements[i] != nullptr)
		{
			Elements[i]->Outer = this;
			Elements[i]->InternalUpdate(DeltaTime);
		}
	}
	Update(DeltaTime);
}

String Drawable::GetName()
{
	return name;
}

Bounds Drawable::GetBounds(bool bNonClient)
{
	Bounds outerBounds = GetOuterBounds(bNonClient || IsNonClient());
	outerBounds.Pos += Position;
	outerBounds.Size = Size;
	outerBounds.Pos.x += Padding.left;
	outerBounds.Pos.y += Padding.top;
	outerBounds.Size.x -= (Padding.right+Padding.left);
	outerBounds.Size.y -= (Padding.bottom+Padding.top);
	return outerBounds;
}

Bounds Drawable::GetSelectionBounds()
{
	Bounds selBounds = GetBounds();
	Bounds clientBounds = GetAppPage()->GetClientBounds();
	if (IsNonClient())
	{
		selBounds = selBounds - clientBounds;
	}
	return selBounds;
}

Bounds Drawable::GetOuterBounds(bool bNonClient)
{
	return GetOuter() != nullptr ? GetOuter()->GetBounds(bNonClient) : appPage->GetBounds(bNonClient);
}

Drawable* Drawable::GetDrawableAtPosition(Vector2D Position)
{
	for (Drawable* element : Elements)
	{
		Bounds elBound = elBound = element->GetSelectionBounds();		
		if (element->IsInteractive() && elBound.IsInBound(Position)) return element->GetDrawableAtPosition(Position);
	}
	return this;
}

void Drawable::OnMouseMove(const Vector2D& Position)
{
	for (Drawable* elem : Elements)
	{
		elem->OnMouseMove(Position);
	}
}

