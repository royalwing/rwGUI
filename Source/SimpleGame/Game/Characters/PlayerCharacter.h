#pragma once

#include "BaseCharacter.h"

class PlayerCharacter : public BaseCharacter
{
	using BaseCharacter::BaseCharacter;

public:
	Vector2D GetAimingLocation() const;
};