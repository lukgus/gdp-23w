#pragma once

#include <ecs/Component.h>

class RigidBody;

struct RigidBodyComponent : public Component
{
	RigidBody* rigidBody;
};