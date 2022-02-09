#include "Projectile.h"
#include "Engine/Components/DynamicSpriteComponent.h"
#include "Engine/Components/PhysicsBodyComponent.h"

Projectile::Projectile(String inName, World* inWorld)
	: Entity(inName, inWorld)
{
	DynamicSpriteComponent* Sprite = CreateComponent<DynamicSpriteComponent>("Sprite");
	Sprite->SetSize({ 8, 8 });
	Sprite->SetSortOrder(15);
	Sprite->SetOnDraw([](ID2D1BitmapRenderTarget* renderTarget)
	{
			ID2D1SolidColorBrush* Brush;
			renderTarget->CreateSolidColorBrush(Color(0, 0, 0, 255).ToD2D1ColorF(), D2D1::BrushProperties(), &Brush);
			const Vector2D Size = renderTarget->GetSize();
			const float BorderWidth = 2.0f;
			D2D1_ELLIPSE ellipse = D2D1::Ellipse(Size / 2, Size.x/2-BorderWidth/2, Size.y/2-BorderWidth/2);
			renderTarget->DrawEllipse(&ellipse, Brush, BorderWidth);
			Brush->Release();
	});

	Body = CreateComponent<PhysicsBodyComponent>("Body");
	Body->SetSphereRadius(10);
	Body->MaxAcceleration = 15000.0f;
	Body->Deceleration = 500.0f;
	SetLifespan(1.0f);
}

void Projectile::OnTick(float DeltaTime)
{
	Body->SetVelocity(Direction*1400.0f);
}
