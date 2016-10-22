#include <rwupd.h>
#include <rwgui.h>
#include <Elements/Drawables/Layout.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Pages/BasePage.h>

class UpdaterGUIApp : public Application
{
public:
	int r = 0;

	char* GetApplicationName() { return "GUI"; };
	void Update(float DeltaTime) override {
		
	};
	Bounds GetDefaultWindowBounds() { return Bounds(200, 200,1280,720); };
	virtual void OnInit() override
	{
		SetMinimalWindowSize(Vector2D(680,360));
	}
	virtual void BuildPages() {
		ApplicationPage* appPage = new BasePage();
		Layout* MainLayout = new Layout("MainLayout", LayoutType_HORIZONTAL);
		appPage->AddElement(MainLayout);
		MainLayout->SetPadding({0,0,0,0});
		MainLayout->SetContentPadding({ 4,8,4,8 });



		Button* ToolsBtn = new Button("ToolsBtn");
		MainLayout->AddChild(ToolsBtn);
		Button* ToolsBtn2 = new Button("ToolsBtn2");
		MainLayout->AddChild(ToolsBtn2);
		Layout* SecondaryLayout = new Layout("AnotherLayout", LayoutType_VERTICAL);
		MainLayout->AddChild(SecondaryLayout);
		Button* ToolsBtn3 = new Button("ToolsBtn3");
		ToolsBtn3->SetPadding(0, 0, 4, 0);
		SecondaryLayout->AddChild(ToolsBtn3);

		Layout* InsideSedLayoyut = new Layout("ToolsBtn4", LayoutType_HORIZONTAL);
		InsideSedLayoyut->SetPadding(4, 0, 8, 0);
		SecondaryLayout->AddChild(InsideSedLayoyut);
		Button* ToolsBtn5 = new Button("ToolsBtn5");
		ToolsBtn5->SetPadding(0, 0, 0, 4);
		InsideSedLayoyut->AddChild(ToolsBtn5);
		Button* ToolsBtn6 = new Button("ToolsBtn6");
		ToolsBtn6->SetPadding(0, 4, 0, 4);
		InsideSedLayoyut->AddChild(ToolsBtn6);

		Layout* InsideSedLayoyut2 = new Layout("ToolsBtn42", LayoutType_HORIZONTAL);
		InsideSedLayoyut2->SetPadding(4, 0, 4, 0);
		SecondaryLayout->AddChild(InsideSedLayoyut2);
		Button* ToolsBtn52 = new Button("ToolsBtn52");
		ToolsBtn52->SetPadding(0, 0, 0, 4);
		InsideSedLayoyut2->AddChild(ToolsBtn52);
		Button* ToolsBtn62 = new Button("ToolsBtn62");
		ToolsBtn62->SetPadding(0, 4, 0, 4);
		InsideSedLayoyut2->AddChild(ToolsBtn62);
		


		MainLayout->zOrder = 0;

		appPage->SetTitle(GetApplicationName());
		AddPage(appPage);
	};
};

RUN_APPLICATION(UpdaterGUIApp);