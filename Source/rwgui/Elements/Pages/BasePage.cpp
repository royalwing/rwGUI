#include "BasePage.h"
#include <Elements/Drawables/Background.h>
#include <Elements/Drawables/Border.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/Header.h>

BasePage::BasePage(char* pageTitle, Color backgroundColor, Color borderColor, int borderWidth)
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
	AddElement(new Button("CloseButton","Close", Bounds(50, 50, 170, 35), [](Application* app) {
		app->Stop();
	}));
	AddElement(new Button("Button2","None", Bounds(240, 50, 170, 35), [](Application* app) {
	}));
	AddElement(new Header("Header", pageTitle));

}