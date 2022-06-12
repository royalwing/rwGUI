#pragma once

#include "Engine.h"

class SimpleBlock : public Entity
{    
	using Entity::Entity;
private:
    class PhysicsBodyComponent* Body = nullptr;
    class DynamicSpriteComponent* Visual = nullptr;
public:

    SimpleBlock(String inName, World* inWorld);
    ~SimpleBlock();

    void SetSize(Vector2D newSize);
    Vector2D GetSize() const;
};