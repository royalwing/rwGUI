#include "PlayerCharacter.h"
#include "rwgui.h"
#include "SimpleGameMain.h"
#include "Engine/Engine.h"

Vector2D PlayerCharacter::GetAimingLocation() const
{
	if(SimpleGame* SimpleGameApp = dynamic_cast<SimpleGame*>(GApplication))
	{
		if (SimpleGameApp->pViewport)
			return SimpleGameApp->pViewport->GetWorldCursorPosition();
	}
	return Vector2D();
}
