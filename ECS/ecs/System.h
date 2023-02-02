#pragma once

#include <vector>

#include <ecs/Entity.h>

class System
{
public:
	virtual void Process(const std::vector<Entity*>& entities, float dt) = 0;
};