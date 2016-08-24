#include "Drawable.h"


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
	return Bounds(0, 0, 0, 0);
}

Bounds Drawable::GetOuterBounds()
{
	return GetOuter() != nullptr ? GetOuter()->GetBounds() : appPage->GetBounds();
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
	result += appPage->GetApplication()->GetCurrentWindowBounds();
	return result;
}

Drawable* Drawable::GetDrawableAtPosition(Vector2D Position)
{
	for (Drawable* element : Elements)
	{
		Bounds elBound = element->GetOuterBounds();
		elBound = elBound + element->GetSelectionBounds();
		if (element->IsInteractive() && elBound.IsInBound(Position)) return element->GetDrawableAtPosition(Position);
	}
	return this;
}

