#include "Layout.h"

Layout::Layout(char * name, ELayoutType Type)
	: Drawable(name)
{
	LayoutType = Type;
	bInteractive = false;
	contentPadding = { 0,0,0,0 };
}

void Layout::Draw(RWD2D * d2d, ID2D1HwndRenderTarget * renderTarget)
{
	Drawable::Draw(d2d, renderTarget);
}

void Layout::Update(float DeltaTime)
{
	Drawable::Update(DeltaTime);
	int numChildren = Elements.size();
	if (numChildren == 0) return;

	if (GetOuter() == nullptr)
	{
		Bounds OuterBounds = GetOuterBounds();
		SetSize(OuterBounds.Size.x, OuterBounds.Size.y);
	}

	Bounds bounds = GetBounds();
	float cellWidth = bounds.Size.x / numChildren - ((contentPadding.left+contentPadding.right) / numChildren);
	float cellHeight = bounds.Size.y / numChildren - ((contentPadding.top + contentPadding.bottom) / numChildren);


	bInteractive = false;
	for (auto e : Elements)
	{
		if (e->IsInteractive())
		{
			bInteractive = true;
			break;
		}
	}

	switch (LayoutType)
	{
	case LayoutType_HORIZONTAL:
		for (int i = 0; i < numChildren;i++)
		{
			Elements[i]->SetPosition(contentPadding.left + i*cellWidth+contentPadding.left, contentPadding.top);
			Elements[i]->SetSize(cellWidth-(contentPadding.right+contentPadding.left), bounds.Size.y-contentPadding.bottom-contentPadding.top);
		}
		break;
	case LayoutType_VERTICAL:
		for (int i = 0; i < numChildren; i++)
		{
			Elements[i]->SetPosition(contentPadding.left, i*cellHeight+contentPadding.top);
			Elements[i]->SetSize(bounds.Size.x - contentPadding.right, cellHeight-contentPadding.bottom-contentPadding.top);
		}
		break;
	}
}

void Layout::SetContentPadding(RECT nPadding)
{
	contentPadding = nPadding;
}
