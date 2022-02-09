#include "SoundEditorApp.h"
#include <Elements/Drawables/Menu.h>

void SoundEditorApp::CreateSoundProject()
{
	if(SoundProjectPtr!=nullptr)
	{
		CloseSoundProject();
	}
	SoundProjectPtr = new SoundProject();
	OnProjectOpened(SoundProjectPtr);
}

void SoundEditorApp::CloseSoundProject()
{
	if (SoundProjectPtr != nullptr)
	{
		delete SoundProjectPtr;
		SoundProjectPtr = nullptr;
		OnProjectClosed();
	}
}

void SoundEditorApp::BuildPages()
{
	MainPagePtr = new MainPage();
	MainPagePtr->SetTitle("Sound Editor");
	Menu* MenuPtr = new Menu("MainMenu");
	(*MenuPtr)["File"]["New project"]["Really new"] = [this]() {
		
	};
	(*MenuPtr)["Edit"];
	(*MenuPtr)["Quit"] = [this]() {
		GApplication->Stop();
	};
	(*MenuPtr)["Test"]["Do some test"];
	MainPagePtr->AddElement(MenuPtr);

	AddPage(MainPagePtr);
}
