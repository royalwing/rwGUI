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
	renderTarget->FillRectangle(GetBounds().ToD2DRect(), borderBrush);
}

void Border::Update(float DeltaTime)
{
	switch (borderType)
	{
	case EBorderType::Top:
		SetPosition(borderWidth, 0);
		SetSize(GetOuterBounds().Size.x - borderWidth * 2, borderWidth);
		break;
	case EBorderType::Bottom:
		SetPosition(borderWidth, GetOuterBounds().Size.y - borderWidth);
		SetSize(GetOuterBounds().Size.x - borderWidth * 2, borderWidth);
		break;
	case EBorderType::Left:
		SetPosition(0, borderWidth);
		SetSize(borderWidth, GetOuterBounds().Size.y - borderWidth * 2);
		break;
	case EBorderType::Right:
		SetPosition(GetOuterBounds().Size.x - borderWidth, borderWidth);
		SetSize(borderWidth, GetOuterBounds().Size.y - borderWidth * 2);
		break;
	case EBorderType::TopLeft:
		SetPosition(0,0);
		SetSize(borderWidth,borderWidth);
		break;
	case EBorderType::TopRight:
		SetPosition(GetOuterBounds().Size.x - borderWidth, 0);
		SetSize(borderWidth, borderWidth);
		break;
	case EBorderType::BottomRight:
		SetPosition(GetOuterBounds().Size.x - borderWidth, GetOuterBounds().Size.y - borderWidth);
		SetSize(borderWidth, borderWidth);
		break;
	case EBorderType::BottomLeft:
		SetPosition(0, GetOuterBounds().Size.y - borderWidth);
		SetSize(borderWidth, borderWidth);
		break;
	}
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
}
HCURSOR Border::GetCursor()
{
	switch (borderType)
	{
	case Bottom:
	case Top: return LoadCursor(nullptr, IDC_SIZENS);
	case Left:
	case Right: return LoadCursor(nullptr, IDC_SIZEWE);
	case TopLeft:
	case BottomRight: return LoadCursor(nullptr, IDC_SIZENWSE);
	case TopRight:
	case BottomLeft: return LoadCursor(nullptr, IDC_SIZENESW);
	}
	return LoadCursor(nullptr,IDC_ARROW);
}
;