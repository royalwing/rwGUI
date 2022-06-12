#include "Crosshair.h"
#include "Characters/PlayerCharacter.h"

void CrosshairComponent::OnDynamicDraw(ID2D1BitmapRenderTarget* renderTarget)
{
	ID2D1SolidColorBrush* Brush;
	renderTarget->CreateSolidColorBrush(Color(1.0f, 0.7f, 0.0f,1.0f).ToD2D1ColorF(), D2D1::BrushProperties(), &Brush);
	Vector2D Size = renderTarget->GetSize();
	float CrosshairWidth = 2.0f;
	renderTarget->DrawLine(Vector2D(Size.x / 2, 0), Vector2D(Size.x / 2, Size.y*0.4f), Brush, CrosshairWidth);
	renderTarget->DrawLine(Vector2D(Size.x / 2, Size.y*0.6f), Vector2D(Size.x / 2, Size.y), Brush, CrosshairWidth);

	renderTarget->DrawLine(Vector2D(0, Size.y / 2), Vector2D(Size.x*0.4f, Size.y / 2), Brush, CrosshairWidth);
	renderTarget->DrawLine(Vector2D(Size.x*0.6f, Size.y / 2), Vector2D(Size.x, Size.y / 2), Brush, CrosshairWidth);
	Brush->Release();
}

Crosshair::Crosshair(String inName, World* inWorld)
	: Entity(inName, inWorld)
{

	SetTickGroup();

	CrosshairComponent* CrosshairComp = CreateComponent<CrosshairComponent>("Crosshair");
	CrosshairComp->SetSortOrder(999);
	CrosshairComp->SetSize({ 24, 24 });
}

void Crosshair::Tick(float DeltaTime, ETickGroup TickGroup)
{
	if(OwnerCharacter!=nullptr)
	{
		SetPosition(OwnerCharacter->GetAimingLocation());
	}
}

void Crosshair::SetOwningCharacter(PlayerCharacter* inCharacter)
{
	if(inCharacter)
	{
		OwnerCharacter = inCharacter;
	}
}

PlayerCharacter* Crosshair::GetOwnerCharacter() const
{
	return OwnerCharacter;
}
