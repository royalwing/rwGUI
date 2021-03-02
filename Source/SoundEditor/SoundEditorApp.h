#pragma once

#include <rwgui.h>
#include "Components/Pages/MainPage.h"
#include <Common/Events.h>
#include <Core\SoundData.h>

class SoundEditorApp : public Application
{
public:
	
	void CreateSoundProject();
	void CloseSoundProject();

	MulticastEvent<SoundProject*> OnProjectOpened;
	MulticastEvent<> OnProjectClosed;
private:
	SoundProject* SoundProjectPtr = nullptr;
	MainPage* MainPagePtr = nullptr;

	String GetApplicationName() const override { return "SoundEditor"; };
	Bounds GetDefaultWindowBounds() override { return Bounds(100, 100, 1280, 720); };
	virtual void OnInit() override
	{
		SetMinimalWindowSize(Vector2D(680, 360));
		Maximize();
	}

	virtual void BuildPages() override;
};
