#pragma once

#include <ecs/Component.h>

struct VelocityComponent : public Component
{
	float vx, vy, vz;
};
