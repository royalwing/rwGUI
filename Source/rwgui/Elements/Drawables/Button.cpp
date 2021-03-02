#include "Button.h"
#include "../../rwgui.h"

Button::Button(String name, std::function<void(Button*)> OnPress)
	: Drawable(name)
{
	bInteractive = true;
	OnButtonPressedEvent.Bind(OnPress);
	BackgroundColor.r = 1.0f;
	BackgroundColor.g = 1.0f;
	BackgroundColor.b = 1.0f;
	BackgroundColor.a = 1.0f;
}

void Button::OnMouseClick()
{
	OnButtonPressedEvent(this);
}

HCURSOR Button::GetCursor()
{
	return LoadCursor(nullptr, IDC_HAND);
}

void Button::SetBackgroundColor(const Color& bgColor)
{
	BackgroundColor = bgColor;
}

void Button::SetBackgroundImage(String bgImagePath)
{
	if (bgImagePath.IsEmpty())
		background = nullptr;
	else
		background = MAKEBITMAP(bgImagePath);
}

void Button::SetBackgroundImage(int ResourceID)
{
	if (ResourceID == 0) return;
	background = MAKEBITMAP(ResourceID);
}

void Button::SetCaption(String caption)
{
	Caption = caption;
}

void Button::Draw(RWD2D* d2d, ID2D1HwndRenderTarget* renderTarget)
{
	IDWriteTextFormat* textFormat = MAKETEXTFORMAT("Arial", fontSize, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_EXTRA_EXPANDED);
	IDWriteTextLayout* textLayout = nullptr;





	Bounds bgBounds;
	if (background != nullptr)
	{
		bgBounds = Bounds(0, 0, background->GetSize().width, background->GetSize().height);
		float width = background->GetSize().width;
		float height = background->GetSize().height;
		switch (BackgroundAlignment)
		{
		case BA_AsIs:
			bgBounds = Bounds(0, 0, GetBounds().Size.x - width, GetBounds().Size.y - height);
			break;
		case BA_StretchToFit:
			bgBounds = Bounds(0, 0, width, height);
			break;
		case BA_ScaleToFit:
			{
				float dilator = GetBounds().Size.y / height;
				
				bgBounds = Bounds(width/2*dilator-GetBounds().Size.x/2, 0, GetBounds().Size.x/dilator, height);
				if (GetBounds().Size.x > GetBounds().Size.y)
				{
					dilator = GetBounds().Size.x / width;
					bgBounds = Bounds(0, height / 2 * dilator - GetBounds().Size.y / 2, width, GetBounds().Size.y / dilator);
				}
			}
			break;
		default:
			break;
		}
	}
	d2d->GetWriteFactory()->CreateTextLayout(Caption.ToWideString(), Caption.Length(), textFormat, GetBounds().Size.x, GetBounds().Size.y, &textLayout);

	DWRITE_TEXT_METRICS metrics;
	if(textLayout != nullptr) textLayout->GetMetrics(&metrics);


	Drawable* curHovered = GetApplication()->GetCurrentHoveredDrawable();
	if (bPressed)
	{
		if(BackgroundColor.a > 0.0f) renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.87f));
		if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 0.9f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, bgBounds.ToD2DRect());
		if (BackgroundColor.a > 0.0f) renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.6f));
		if(textLayout != nullptr) renderTarget->DrawText(Caption.ToWideString(), Caption.Length(), textFormat, Bounds(GetBounds().Pos.x + GetBounds().Size.x / 2 - metrics.width / 2, GetBounds().Pos.y + GetBounds().Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(0.0f, 0.0f, 0.0f)));
	}
	else
	{
		if (curHovered != nullptr &&  curHovered == this)
		{
			if (BackgroundColor.a > 0.0f) renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.8f));
			if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, bgBounds.ToD2DRect());
			if (BackgroundColor.a > 0.0f) renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor));
			if (textLayout != nullptr) renderTarget->DrawText(Caption.ToWideString(), Caption.Length(), textFormat, Bounds(GetBounds().Pos.x + GetBounds().Size.x / 2 - metrics.width / 2, GetBounds().Pos.y + GetBounds().Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
		}
		else
		{
			if (BackgroundColor.a > 0.0f) renderTarget->FillRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor*0.7f));
			if (background != nullptr) renderTarget->DrawBitmap(background, GetBounds().ToD2DRect(), 0.5f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, bgBounds.ToD2DRect());
			if (BackgroundColor.a > 0.0f) renderTarget->DrawRectangle(GetBounds().ToD2DRect(), MAKEBRUSH(BackgroundColor));
			if (textLayout != nullptr) renderTarget->DrawText(Caption.ToWideString(), Caption.Length(), textFormat, Bounds(GetBounds().Pos.x + GetBounds().Size.x / 2 - metrics.width / 2, GetBounds().Pos.y + GetBounds().Size.y / 2 - metrics.height / 2, metrics.width, metrics.height).ToD2DRect(), MAKEBRUSH(Color(1.0f, 1.0f, 1.0f)));
		}
	}
	if (textLayout != nullptr)
	{
		textLayout->Release();
	}

	Drawable::Draw(d2d, renderTarget);
}
