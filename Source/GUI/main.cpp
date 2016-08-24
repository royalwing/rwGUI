#include <rwupd.h>
#include <rwgui.h>

class UpdaterGUIApp : public Application
{
public:
	char* GetApplicationName() { return "Updater"; };
	bool Update() { return true; };
	Bounds GetDefaultWindowBounds() { return Bounds(200, 200,1280,720); };
	virtual void OnInit() override
	{
	}
	virtual void BuildPages(std::vector<ApplicationPage*>& outPages) {

		ApplicationPage* appPage = new BasePage(GetApplicationName());
		outPages.push_back(appPage);
	};
};

RUN_APPLICATION(UpdaterGUIApp);