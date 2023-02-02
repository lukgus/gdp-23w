#pragma once

#include <ecs/Component.h>

struct PositionComponent : public Component
{
	float x, y, z;
};