#include "Border.h"

Border::Border(char* name, EBorderType type, Color color, int width)
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
		curBounds.Size.x = outerBounds.Size.x - borderWidth * 2;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::Bottom:
		curBounds.Pos.x = borderWidth;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth;
		curBounds.Size.x = outerBounds.Size.x - borderWidth * 2;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::Left:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = outerBounds.Size.y - borderWidth * 2;
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
		curBounds.Pos.x = outerBounds.Size.x - borderWidth;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth;
		curBounds.Size.x = borderWidth;
		curBounds.Size.y = borderWidth;
		break;
	case EBorderType::BottomLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth;
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
		curBounds.Size.x = outerBounds.Size.x - (borderWidth + selectionborder) * 2;
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
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = borderWidth + selectionborder;
		break;
	case EBorderType::TopRight:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth - selectionborder;
		curBounds.Pos.y = 0;
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = borderWidth + selectionborder;
		break;
	case EBorderType::BottomRight:
		curBounds.Pos.x = outerBounds.Size.x - borderWidth - selectionborder;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth - selectionborder;
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = borderWidth + selectionborder;
		break;
	case EBorderType::BottomLeft:
		curBounds.Pos.x = 0;
		curBounds.Pos.y = outerBounds.Size.y - borderWidth - selectionborder;
		curBounds.Size.x = borderWidth + selectionborder;
		curBounds.Size.y = borderWidth + selectionborder;
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