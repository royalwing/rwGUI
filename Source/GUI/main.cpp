#include <rwupd.h>
#include <rwgui.h>
#include <Elements/Pages/BasePage.h>

class UpdaterGUIApp : public Application
{
public:
	char* GetApplicationName() { return "GUI"; };
	void Update(float DeltaTime) override {
		Drawable* drawable = GetCurrentHoveredDrawable();
		if(drawable==nullptr) GetActivePage()->SetTitle("None");
		else GetActivePage()->SetTitle(drawable->GetName());
	};
	Bounds GetDefaultWindowBounds() { return Bounds(200, 200,1280,720); };
	virtual void OnInit() override
	{
		SetMinimalWindowSize(Vector2D(680,360));
	}
	virtual void BuildPages() {

		ApplicationPage* appPage = new BasePage();
		appPage->SetTitle(GetApplicationName());
		AddPage(appPage);
	};
};

RUN_APPLICATION(UpdaterGUIApp);