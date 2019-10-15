
#include "Common/DebugHelpers.h"
#include "SimpleGameMain.h"
#include "Elements/Pages/BasePage.h"
#include "Engine/Engine.h"
#include "Engine/Components/SpriteComponent.h"

RUN_APPLICATION(SimpleGame);

void SimpleGame::OnInit()
{
	SetMinimalWindowSize(Vector2D(800, 600));

	if(pWorld = Engine::Get()->CreateWorld())
		pViewport = new Viewport("GameViewport", pWorld);


	Entity* pEntity = pWorld->SpawnEntity<Entity>("SomeObject");
	pEntity->RegisterComponent(new SpriteComponent("Sprite", pEntity, "D:/test.jpg"));

	pEntity->TickDelegate = [](Object* Object, float DeltaTime)
	{
		if(Entity* pEntity = dynamic_cast<Entity*>(Object))
		{
			pEntity->SetPosition(pEntity->GetPosition()+Vector2D(cos(GApplication->GetRealTimeSeconds()), sin(GApplication->GetRealTimeSeconds())));
		}
	};
}

void SimpleGame::Update(float DeltaTime)
{
	pViewport->SetPosition(pViewport->GetPosition() + Vector2D(DeltaTime * 200.0f , 0));

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

