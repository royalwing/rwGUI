
#include "Common/DebugHelpers.h"
#include "SimpleGameMain.h"
#include "Elements/Pages/BasePage.h"
#include "Engine/Engine.h"

RUN_APPLICATION(SimpleGame);

void SimpleGame::OnInit()
{
	SetMinimalWindowSize(Vector2D(800, 600));

	if(pWorld = Engine::Get()->CreateWorld())
		pViewport = new Viewport("GameViewport", pWorld);	
}

void SimpleGame::Update(float DeltaTime)
{
	Engine::Get()->Tick(DeltaTime);
}

void SimpleGame::OnStop()
{
	Engine::Get()->Stop();
}

void SimpleGame::BuildPages()
{
	BasePage* MainPage = new BasePage();
	MainPage->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f));
	MainPage->SetTitle("SimpleGame");
	MainPage->SetBorderColor(Color(0.79f, 0.317f, 0.0f, 1.0f));
	MainPage->AddElement(pViewport);
	
	AddPage(MainPage);
}

