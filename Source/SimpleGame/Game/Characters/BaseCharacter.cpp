#include "BaseCharacter.h"
#include "Engine/Components/SpriteComponent.h"
#include "Engine/Components/PhysicsBodyComponent.h"
#include "Engine/Components/DynamicSpriteComponent.h"

BaseCharacter::BaseCharacter(String Name, World* inWorld)
	: Entity(Name, inWorld)
{
	
	DynamicSpriteComponent* Sprite = new DynamicSpriteComponent("Sprite", this);
	RegisterComponent(Sprite);
	Sprite->SetSize({ 64,64 });
	Sprite->SetSortOrder(1);
	Sprite->SetOnDraw([](ID2D1BitmapRenderTarget* renderTarget)
	{
		ID2D1SolidColorBrush* Brush;
		renderTarget->CreateSolidColorBrush(Color(0, 0, 0, 255).ToD2D1ColorF(), D2D1::BrushProperties(), &Brush);
		const Vector2D Size = renderTarget->GetSize();
		float OuterBorderWidth = 1.0f;
		float InnerBorderWidth = 12.0f;
		float OuterSize = 0.5f;
		float InnerSize = 0.45f;
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(Size / 2, Size.x * InnerSize - InnerBorderWidth / 2, Size.y * InnerSize - InnerBorderWidth / 2);
		renderTarget->DrawEllipse(&ellipse, Brush, InnerBorderWidth);
		ellipse = D2D1::Ellipse(Size / 2, Size.x * OuterSize - OuterBorderWidth / 2, Size.y * OuterSize - OuterBorderWidth / 2);
		renderTarget->DrawEllipse(&ellipse, Brush, OuterBorderWidth);
		Brush->Release();
	});

	Physics = CreateComponent<PhysicsBodyComponent>("Body");
	Physics->SetSphereRadius(40.0f);

	MovementSpeed = 400.0f;
}

BaseCharacter::~BaseCharacter()
{
}


void BaseCharacter::AddAcceleration(const Vector2D& inAcceleration)
{
	Physics->AddAcceleration(inAcceleration);
}

void BaseCharacter::OnTick(float DeltaTime)
{
	Physics->SetVelocity(MovementInput * MovementSpeed);
}
