#ifndef BASEPAGE_H
#define BASEPAGE_H

class Header;

#include "ApplicationPage.h"
#include <Elements/Drawables/Background.h>
#include <Elements/Drawables/Border.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/Header.h>


class RWGUI_API BasePage : public ApplicationPage
{
private:
	Color borderColor = Color(0.6f, 0.6f, 0.6f);
	Color backgroundColor = Color(0.9f, 0.9f, 0.9f);
	int borderWidth = 1;
	Header* header = nullptr;
	Background* background = nullptr;
public:
	virtual void BuildPage() override;
	virtual void OnInit() override;
	virtual void SetTitle(char* newtitle) override;
	virtual Bounds GetClientBounds() override;

};


#endif