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
	//renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(Color(1.0f, 0.0f, 0.0f)), 1.0f);
}

void Layout::Update(float DeltaTime)
{
	Drawable::Update(DeltaTime);
	int numChildren = Elements.size();
	if (numChildren == 0) return;
		
	Layout* OuterLayout = nullptr;
	if (GetOuter() != nullptr)
	{
		OuterLayout = dynamic_cast<Layout*>(GetOuter());
	}

	if (GetOuter() == nullptr || OuterLayout == nullptr)
	{
		Bounds OuterBounds = GetOuterBounds();
		SetSize(OuterBounds.Size.x, OuterBounds.Size.y);
	}

	bInteractive = false;
	for (auto e : Elements)
	{
		if (e->IsInteractive())
		{
			bInteractive = true;
			break;
		}
	}



	Bounds bounds = GetBounds();

	float ScaleSumm = 0;
	for (int i = 0; i < numChildren; i++)
	{
		ScaleSumm += Elements[i]->LayoutScale;
	}


	Vector2D LastPosition, LastSize;
	LastPosition.x = contentPadding.left;
	LastPosition.y = contentPadding.top;
	
	for (int i = 0; i < numChildren;i++)
	{
		float CurrentScale = 1.0f / ScaleSumm*Elements[i]->LayoutScale;
		Vector2D CurrentLocation = LastPosition;
		Vector2D CurrentSize = LastSize;
		switch (LayoutType)
		{
		case LayoutType_HORIZONTAL:
			if (!Elements[i]->bIgnoreLayoutScaling)
			{
				CurrentSize.x = bounds.Size.x*CurrentScale - contentPadding.left - contentPadding.right;
				CurrentSize.y = bounds.Size.y - contentPadding.top - contentPadding.bottom;
			}
			else {
				CurrentSize = Elements[i]->GetBounds().Size;
			}
			CurrentLocation.x = LastSize.x + LastPosition.x + contentPadding.right;
			break;
		case LayoutType_VERTICAL:
			if (!Elements[i]->bIgnoreLayoutScaling)
			{
				CurrentSize.x = bounds.Size.x - contentPadding.left - contentPadding.right;
				CurrentSize.y = bounds.Size.y*CurrentScale - contentPadding.top - contentPadding.bottom;
			}
			else {
				CurrentSize = Elements[i]->GetBounds().Size;
			}
			CurrentLocation.y = LastSize.y + contentPadding.top + LastPosition.y + contentPadding.bottom;
			break;
		}
		Elements[i]->SetPosition(CurrentLocation.x, CurrentLocation.y);
		Elements[i]->SetSize(CurrentSize.x, CurrentSize.y);
		LastPosition = CurrentLocation;
		LastSize = CurrentSize;
	}

}

void Layout::SetContentPadding(RECT nPadding)
{
	contentPadding = nPadding;
}

void Layout::SetContentPadding(int top, int left, int bottom, int right)
{
	SetContentPadding({left,top,right,bottom});
}

void Layout::SetContentVerticalAlignment(ELayoutVertialContentAlignment newVerticalAlignment)
{
	contentVerticalAlignment = newVerticalAlignment;
}

void Layout::SetContentHorizontalAlignment(ELayoutHorizontalContentAlignment newHorizontalAlignment)
{
	contentHorizontalAlignment = newHorizontalAlignment;
}
