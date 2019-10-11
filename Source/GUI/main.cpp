#include <rwupd.h>
#include <rwgui.h>
#include <Elements/Drawables/Layout.h>
#include <Elements/Drawables/Button.h>
#include <Elements/Drawables/TextLabel.h>
#include <Elements/Drawables/Spacer.h>
#include <Elements/Pages/BasePage.h>
#include <Common/DebugHelpers.h>
#include "resource.h"
#include "Common/Containers.h"

class UpdaterGUIApp : public Application
{
public:
	int r = 0;

	String GetApplicationName() const override { return "GUI"; };
	void Update(float DeltaTime) override {
		
	};
	Bounds GetDefaultWindowBounds() override { return {200, 200,1280,720}; };
	virtual void OnInit() override
	{
		SetMinimalWindowSize(Vector2D(680,360));
	}
	virtual void BuildPages() override
	{

		List<int> TestArray;
		TestArray.Insert(32, 0);
		TestArray.Insert(12, 0);
		TestArray.Insert(49, 0);
		TestArray.Insert(81, 1);

		TestArray.RemoveAt(1);

		for(int i : TestArray)
		{
			RW_LOG(std::to_string(i).c_str());
		}

		BasePage* MainPage = new BasePage();
		Layout* MainLayout = new Layout("MainLayout", LayoutType_HORIZONTAL);
		MainPage->AddElement(MainLayout);
		MainLayout->SetPadding({0,0,0,0});
		MainLayout->SetContentPadding(0,4,4,8);


		// AboutMe button
		{
			Button* AboutMeBtn = new Button("AboutMeBtn", [](Application* app) {
				app->SetActivePage(1);
			});
			//AboutMeBtn->SetBackgroundImage(IDB_BACKGROUND_ABOUT);
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
			AboutMeBtnCaption->SetHorizontalAlignment(ETextHorizontalAlignment::THA_LEFT);
			AboutMeBtnCaption->SetFontStyle(ETextStyle::TS_Bold);
			AboutMeBtnCaption->LayoutScale = 0.2f;
			AboutMeBtnLayout->AddChild(AboutMeBtnCaption);

			TextLabel* AboutMeBtnDescription = new TextLabel("AboutMeBtnDescription", "Here you can check all the information about my skills and how to contact me.");
			AboutMeBtnDescription->SetFontSize(12);
			AboutMeBtnDescription->SetHorizontalAlignment(ETextHorizontalAlignment::THA_LEFT);
			AboutMeBtnDescription->SetFontStyle(ETextStyle::TS_Italic);
			AboutMeBtnDescription->LayoutScale = 0.35f;
			AboutMeBtnLayout->AddChild(AboutMeBtnDescription);
		}
		
		
		// AppsBtn button
		{
			Button* Apps_Btn = new Button("AppsBtn");
			//Apps_Btn->SetBackgroundImage(IDB_BACKGROUND_APPS);
			Apps_Btn->BackgroundAlignment = Button::EBackgroundAlignment::BA_ScaleToFit;
			MainLayout->AddChild(Apps_Btn);

			Layout* AppsBtnLayout = new Layout("AppsBtnLayout", ELayoutType::LayoutType_VERTICAL);
			AppsBtnLayout->SetPadding(15, 15, 15, 15);
			AppsBtnLayout->SetContentPadding(0, 0, 0, 0);
			AppsBtnLayout->SetContentVerticalAlignment(LVCA_Bottom);
			Apps_Btn->AddChild(AppsBtnLayout);

			TextLabel* AppsBtnCaption = new TextLabel("AppsBtnCaption", "My Software");
			AppsBtnCaption->SetFontStyle(ETextStyle::TS_Bold);
			AppsBtnCaption->SetVerticalAlignment(ETextVerticalAlignment::TVA_TOP);
			AppsBtnCaption->SetHorizontalAlignment(ETextHorizontalAlignment::THA_RIGHT);
			AppsBtnCaption->LayoutScale = 0.2f;
			AppsBtnLayout->AddChild(AppsBtnCaption);

			TextLabel* AppsBtnDescription = new TextLabel("AppsBtnDescription", "This section dedicated to observing and downloading apps i'm making. You can update, download, launch and remove them within this section.");
			AppsBtnDescription->SetFontSize(12);
			AppsBtnDescription->SetFontStyle(ETextStyle::TS_Italic);
			AppsBtnDescription->SetVerticalAlignment(ETextVerticalAlignment::TVA_TOP);
			AppsBtnDescription->SetHorizontalAlignment(ETextHorizontalAlignment::THA_RIGHT);
			AppsBtnDescription->LayoutScale = 0.35f;
			AppsBtnLayout->AddChild(AppsBtnDescription);

			Spacer* AppsBtnSpacer = new Spacer("AboutMeBtnSpacer");
			AppsBtnSpacer->LayoutScale = 3.0f;
			AppsBtnLayout->AddChild(AppsBtnSpacer);

			TextLabel* AppsCountBtnDescription = new TextLabel("AppsCountBtnDescription", "Current entries : 0");
			AppsCountBtnDescription->SetFontSize(18);
			AppsCountBtnDescription->SetTextColor(Color(0.0f,0.0f,0.0f));
			AppsCountBtnDescription->SetFontStyle(ETextStyle::TS_Bold);
			AppsCountBtnDescription->SetHorizontalAlignment(ETextHorizontalAlignment::THA_LEFT);
			AppsCountBtnDescription->SetVerticalAlignment(ETextVerticalAlignment::TVA_BOTTOM);
			AppsCountBtnDescription->LayoutScale = 0.35f;
			AppsBtnLayout->AddChild(AppsCountBtnDescription);

		}

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
		
		MainPage->SetTitle("RoyalWing's Updater");
		AddPage(MainPage); // # 0 - Main Page

		BasePage* AboutMePage = new BasePage();
		AboutMePage->SetBackgroundColor((Colors::Green + (Colors::Yellow-Colors::Green)*0.4f)*0.3f);
		AboutMePage->SetTitle("About Me");

		AddPage(AboutMePage); // # 1 - About me page

	};
};



RUN_APPLICATION(UpdaterGUIApp);