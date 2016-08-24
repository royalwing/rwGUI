#ifndef BASEPAGE_H
#define BASEPAGE_H

#include "ApplicationPage.h"

class RWGUI_API BasePage : public ApplicationPage
{
public:
	BasePage(char* pageTitle, Color backgroundColor = Color(0.9f, 0.9f, 0.9f), Color borderColor = Color(0.75f, 0.75f, 0.75f), int borderWidth = 1);
};


#endif