#pragma once

#include <engine/ecs/Component.h>
#include <physics/rigidbody.h>

struct RigidBodyComponent : public Component
{
	RigidBody* rigidBody;
};