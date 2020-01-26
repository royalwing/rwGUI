
#include "Common/DebugHelpers.h"
#include "SimpleGameMain.h"
#include "Elements/Pages/BasePage.h"
#include "Engine/Engine.h"
#include "Game/Characters/PlayerCharacter.h"
#include "Engine/Components/SpriteComponent.h"

RUN_APPLICATION(SimpleGame);

void SimpleGame::OnInit()
{
	SetMinimalWindowSize(Vector2D(800, 600));

	if(pWorld = Engine::Get()->CreateWorld())
	{
		pViewport = new Viewport("GameViewport", pWorld);
		pViewport->SetClearColor(Color(1.0, 1.0, 1.0, 1.0));		
	}

	Entity* pEntity = pWorld->SpawnEntity<Entity>("Grid");
	SpriteComponent* Sprite = pEntity->CreateComponent<SpriteComponent>("GridSprite");
	Sprite->SetSpriteBitmap("D:/grid.png");
	Sprite->SetSpriteSize({512, 512});

	LocalPlayerCharacter = pWorld->SpawnEntity<PlayerCharacter>("LocalPlayerCharacter");
}

void SimpleGame::Update(float DeltaTime)
{
	Engine::Get()->Tick(DeltaTime);
	pViewport->SetWorldPosition(pViewport->GetWorldPosition() + (LocalPlayerCharacter->GetPosition()- pViewport->GetWorldPosition())*3.0f*DeltaTime);

	LocalPlayerCharacter->SetMovementInput(Input);
}

void SimpleGame::OnStop()
{
	Engine::Get()->Stop();
	delete pViewport;
}

void SimpleGame::OnKeyStateChanged(char key, bool bPressed)
{
	if(bPressed)
	{
		switch(key)
		{
		case 'W':
			Input.y += 1;
			break;
		case 'S':
			Input.y -= 1;
			break;
		case 'D':
			Input.x += 1;
			break;
		case 'A':
			Input.x -= 1;
			break;
		default:
			break;
		}
	} else
	{
		switch (key)
		{
		case 'W':
			Input.y -= 1;
			break;
		case 'S':
			Input.y += 1;
			break;
		case 'D':
			Input.x -= 1;
			break;
		case 'A':
			Input.x += 1;
			break;
		default:
			break;
		}
	}
}

void SimpleGame::BuildPages()
{
	BasePage* MainPage = new BasePage();
	MainPage->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f));
	MainPage->SetTitle("SimpleGame");
	MainPage->SetBorderColor(Color(0.79f, 0.317f, 0.0f, 1.0f));



	Layout* pGameLayout = new Layout("GameLayout", ELayoutType::LayoutType_ASIS);
	MainPage->AddElement(pGameLayout);
	pGameLayout->AddChild(pViewport);

	Layout* pUILayout = new Layout("UILayout", LayoutType_VERTICAL);
	pGameLayout->AddChild(pUILayout);





	AddPage(MainPage);
}

