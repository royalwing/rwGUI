#ifndef BASEPAGE_H
#define BASEPAGE_H

class Header;

#include "ApplicationPage.h"


class RWGUI_API BasePage : public ApplicationPage
{
private:
	Color borderColor = Color(0.6f, 0.6f, 0.6f);
	Color backgroundColor = Color(0.9f, 0.9f, 0.9f);
	int borderWidth = 1;
	char* backgroundPath = "D:\\Images\\8462.jpg";
	char* closeBtnPath = "D:\\Images\\close_3.png";
	Header* header = nullptr;
public:
	virtual void BuildPage() override;
	virtual void SetTitle(char* newtitle) override;
};


#endif