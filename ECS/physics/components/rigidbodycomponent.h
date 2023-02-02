#pragma once

#include <engine/ecs/Component.h>
#include <physics/bodies/rigidbody.h>

struct RigidBodyComponent : public Component
{
	RigidBody* rigidBody;
};