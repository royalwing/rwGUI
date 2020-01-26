#pragma once

#include "../Object.h"

enum EPhysicsBodyType
{
	PBT_Circle,
	PBT_Box,
	PBT_Capsule,
	PBT_Rectangle
};

class PhysicsBodyComponent : public Component
{
public:
	PhysicsBodyComponent(String inName, Entity* inOwner, EPhysicsBodyType inType);
};