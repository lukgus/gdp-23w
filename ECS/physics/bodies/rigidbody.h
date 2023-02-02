#pragma once

#include <physics/bodies/collisionbody.h>

#include <physics/shapes/shape.h>

class RigidBody : public CollisionBody
{
public:
	RigidBody();
	virtual ~RigidBody();

	Shape* m_Shape;
};