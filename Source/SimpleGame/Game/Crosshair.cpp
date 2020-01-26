#include "Crosshair.h"
#include "Characters/PlayerCharacter.h"

void CrosshairComponent::OnDynamicDraw(ID2D1BitmapRenderTarget* renderTarget)
{
	ID2D1SolidColorBrush* Brush;
	renderTarget->CreateSolidColorBrush(Color(255, 0, 0, 255).ToD2D1ColorF(), D2D1::BrushProperties(), &Brush);
	Vector2D Size = renderTarget->GetSize();
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(Size/2, 16.0f, 16.0f);
	renderTarget->DrawEllipse(&ellipse, Brush, 4.0f);
	Brush->Release();
}

Crosshair::Crosshair(String inName, World* inWorld)
	: Entity(inName, inWorld)
{
	CrosshairComponent* CrosshairComp = CreateComponent<CrosshairComponent>("Crosshair");
	CrosshairComp->SetSortOrder(999);
	CrosshairComp->SetSize({ 128, 128 });
}

void Crosshair::OnTick(float DeltaTime)
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
