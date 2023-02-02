#pragma once

#include <ecs/System.h>

class RenderSystem : public System
{
public:
	virtual void Process(const std::vector<Entity*>& entities, float dt) { }
};