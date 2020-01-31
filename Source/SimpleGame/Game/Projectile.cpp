#include "Projectile.h"
#include "Engine/Components/DynamicSpriteComponent.h"
#include "Engine/Components/PhysicsBodyComponent.h"

Projectile::Projectile(String inName, World* inWorld)
	: Entity(inName, inWorld)
{
	DynamicSpriteComponent* Sprite = CreateComponent<DynamicSpriteComponent>("Sprite");
	Sprite->SetSize({ 32, 32 });
	Sprite->SetSortOrder(15);
	Sprite->SetOnDraw([](ID2D1BitmapRenderTarget* renderTarget)
	{
			ID2D1SolidColorBrush* Brush;
			renderTarget->CreateSolidColorBrush(Color(0, 0, 0, 255).ToD2D1ColorF(), D2D1::BrushProperties(), &Brush);
			const Vector2D Size = renderTarget->GetSize();
			D2D1_ELLIPSE ellipse = D2D1::Ellipse(Size / 2, 10.0f, 10.0f);
			renderTarget->DrawEllipse(&ellipse, Brush, 2.0f);
			Brush->Release();
	});

	Body = CreateComponent<PhysicsBodyComponent>("Body");
	Body->SetSphereRadius(10);
	Body->MaxAcceleration = 15000.0f;
	Body->Deceleration = 500.0f;
}

void Projectile::OnTick(float DeltaTime)
{
	Body->AddAcceleration(Direction*4000000);
}
