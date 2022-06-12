#include "PlayerCharacter.h"
#include "rwgui.h"
#include "SimpleGameMain.h"
#include "Engine/Engine.h"
#include "Game/Projectile.h"

Vector2D PlayerCharacter::GetAimingLocation() const
{
	if(SimpleGame* SimpleGameApp = dynamic_cast<SimpleGame*>(GApplication))
	{
		if (SimpleGameApp->pViewport)
			return SimpleGameApp->pViewport->GetWorldCursorPosition();
	}
	return Vector2D();
}

void PlayerCharacter::Tick(float DeltaTime, ETickGroup TickGroup)
{
	BaseCharacter::Tick(DeltaTime, TickGroup);
	TimeSinceLastFire += DeltaTime;
	if (TimeSinceLastFire > (1.0f / FireRate) && bWantsToShoot)
	{
		Shoot();
		TimeSinceLastFire = 0.0f;
	}
}

void PlayerCharacter::Shoot()
{
	Projectile* _projectile = GetWorld()->SpawnEntity<Projectile>("Bullet");
	_projectile->SetPosition(GetPosition() + (GetAimingLocation()-GetPosition()).GetNormalized()*48.0f);
	_projectile->Direction = (GetAimingLocation() - GetPosition()).GetNormalized();
	_projectile->SetLifespan(1.3f);
}
