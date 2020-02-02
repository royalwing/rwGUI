
#include "Common/DebugHelpers.h"
#include "SimpleGameMain.h"
#include "Elements/Pages/BasePage.h"
#include "Engine/Engine.h"
#include "Game/Characters/PlayerCharacter.h"
#include "Engine/Components/SpriteComponent.h"
#include "Game/Crosshair.h"

RUN_APPLICATION(SimpleGame);

void SimpleGame::OnInit()
{
	SetMinimalWindowSize(Vector2D(1280, 720));

	if(pWorld = Engine::Get()->CreateWorld())
	{
		pViewport = new Viewport("GameViewport", pWorld);
		pViewport->SetClearColor(Color(1.0, 1.0, 1.0, 1.0));		
	}

	Entity* pEntity = pWorld->SpawnEntity<Entity>("Grid");
	DynamicSpriteComponent* Sprite = pEntity->CreateComponent<DynamicSpriteComponent>("GridSprite");
	Sprite->SetSize({2048, 2048});
	Sprite->SetOnDraw([](ID2D1BitmapRenderTarget* renderTarget)
	{
		ID2D1SolidColorBrush *BrushWhite, *BrushRed, *BrushGreen;
		renderTarget->CreateSolidColorBrush(Color(0, 0, 0, 0.1f).ToD2D1ColorF(), D2D1::BrushProperties(), &BrushWhite);
		renderTarget->CreateSolidColorBrush(Color(1.0f, 0, 0, 0.5f).ToD2D1ColorF(), D2D1::BrushProperties(), &BrushRed);
		renderTarget->CreateSolidColorBrush(Color(0, 1.0f, 0, 0.5f).ToD2D1ColorF(), D2D1::BrushProperties(), &BrushGreen);
		const Vector2D Size = renderTarget->GetSize();
		for(int x = 0; x < Size.x;x+=32)
		{
			if(x>0)
				renderTarget->DrawLine(Vector2D(x, 0), Vector2D(x, Size.y), x % 256 == 0 ? BrushGreen : BrushWhite, 1.0f);
		}
		for (int y = 0; y < Size.y; y += 32)
		{
			if(y>0)
				renderTarget->DrawLine(Vector2D(0, y), Vector2D(Size.x, y), y%256==0? BrushRed : BrushWhite, 1.0f);
		}
		BrushWhite->Release();
		BrushRed->Release();
		BrushGreen->Release();
	});


	LocalPlayerCharacter = pWorld->SpawnEntity<PlayerCharacter>("LocalPlayerCharacter");

	Crosshair* pCrosshair = pWorld->SpawnEntity<Crosshair>("LocalPlayerChrosshair");
	pCrosshair->SetOwningCharacter(LocalPlayerCharacter);
}

void SimpleGame::Update(float DeltaTime)
{
	Engine::Get()->Tick(DeltaTime);
	pViewport->SetWorldPosition(pViewport->GetWorldPosition() + (LocalPlayerCharacter->GetPosition()- pViewport->GetWorldPosition())*6.0f*DeltaTime);

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
		case VK_LBUTTON:
			LocalPlayerCharacter->StartShooting();
			break;
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
		case VK_LBUTTON:
			LocalPlayerCharacter->StopShooting();
			break;
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

