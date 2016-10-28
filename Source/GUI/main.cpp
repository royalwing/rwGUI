#include <rwupd.h>
#include <rwgui.h>
#include <Elements/Drawables/Layout.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/TextLabel.h>
#include <Elements/Drawables/Spacer.h>
#include <Elements/Pages/BasePage.h>
#include <Common/DebugHelpers.h>
#include "resources.h"

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
		GetActivePage()->SetTitle("RoyalWing's Updater");
		SetMinimalWindowSize(Vector2D(680,360));
	}
	virtual void BuildPages() {
		ApplicationPage* appPage = new BasePage();
		Layout* MainLayout = new Layout("MainLayout", LayoutType_HORIZONTAL);
		appPage->AddElement(MainLayout);
		MainLayout->SetPadding({0,0,0,0});
		MainLayout->SetContentPadding(0,4,4,8);


		// AboutMe button
		{
			Button* AboutMeBtn = new Button("AboutMeBtn", [](Application* app) {
				RW_WARNING("Test");
			});
			AboutMeBtn->SetBackgroundImage(IDB_BACKGROUND_ABOUT);
			AboutMeBtn->BackgroundAlignment = Button::EBackgroundAlignment::BA_ScaleToFit;
			MainLayout->AddChild(AboutMeBtn);

			Layout* AboutMeBtnLayout = new Layout("AboutMeBtnLayout", ELayoutType::LayoutType_VERTICAL);
			AboutMeBtnLayout->SetPadding(15, 15, 15, 15);
			AboutMeBtnLayout->SetContentPadding(0, 0, 0, 0);
			AboutMeBtnLayout->SetContentVerticalAlignment(LVCA_Bottom);
			AboutMeBtn->AddChild(AboutMeBtnLayout);

			Spacer* AboutMeBtnSpacer = new Spacer("AboutMeBtnSpacer");
			AboutMeBtnSpacer->LayoutScale = 3.0f;
			AboutMeBtnLayout->AddChild(AboutMeBtnSpacer);

			TextLabel* AboutMeBtnCaption = new TextLabel("AboutMeBtnCaption", "About Me");
			AboutMeBtnCaption->SetFontStyle(ETextStyle::TS_Bold);
			AboutMeBtnCaption->LayoutScale = 0.2f;
			AboutMeBtnLayout->AddChild(AboutMeBtnCaption);

			TextLabel* AboutMeBtnDescription = new TextLabel("AboutMeBtnDescription", "Here you can check all the information about my skills and how to contact me.");
			AboutMeBtnDescription->SetFontSize(12);
			AboutMeBtnDescription->SetFontStyle(ETextStyle::TS_Italic);
			AboutMeBtnDescription->LayoutScale = 0.35f;
			AboutMeBtnLayout->AddChild(AboutMeBtnDescription);
		}
		
		

		Button* ToolsBtn2 = new Button("ToolsBtn2");
		MainLayout->AddChild(ToolsBtn2);

		Layout* SecondaryLayout = new Layout("AnotherLayout", LayoutType_VERTICAL);
		MainLayout->AddChild(SecondaryLayout);
		Button* ToolsBtn3 = new Button("ToolsBtn3");
		ToolsBtn3->SetPadding(0, 0, 4, 0);
		SecondaryLayout->AddChild(ToolsBtn3);

		Layout* InsideSedLayoyut = new Layout("ToolsBtn4", LayoutType_HORIZONTAL);
		SecondaryLayout->AddChild(InsideSedLayoyut);
		Button* ToolsBtn5 = new Button("ToolsBtn5");
		ToolsBtn5->SetPadding(4, 0, 4, 4);
		InsideSedLayoyut->AddChild(ToolsBtn5);
		Button* ToolsBtn6 = new Button("ToolsBtn6");
		ToolsBtn6->SetPadding(4, 4, 4, 0);
		InsideSedLayoyut->AddChild(ToolsBtn6);

		Layout* InsideSedLayoyut2 = new Layout("ToolsBtn42", LayoutType_HORIZONTAL);
		SecondaryLayout->AddChild(InsideSedLayoyut2);
		Button* ToolsBtn52 = new Button("ToolsBtn52");
		ToolsBtn52->SetPadding(4, 0, 0, 4);
		InsideSedLayoyut2->AddChild(ToolsBtn52);
		Button* ToolsBtn62 = new Button("ToolsBtn62");
		ToolsBtn62->SetPadding(4, 4, 0, 0);
		InsideSedLayoyut2->AddChild(ToolsBtn62);
		


		MainLayout->zOrder = 0;

		appPage->SetTitle(GetApplicationName());
		AddPage(appPage);
	};
};

RUN_APPLICATION(UpdaterGUIApp);