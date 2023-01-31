#pragma once

#include "Math.h"
#include "iRigidBody.h"

class iPhysicsWorld
{
public:
	virtual ~iPhysicsWorld() {}

	virtual void SetGravity(const Vector3& gravity) = 0;

	virtual void AddRigidBody(iRigidBody* body) = 0;
	virtual void RemoveRigidBody(iRigidBody* body) = 0;

	virtual void TimeStep(float dt) = 0;

protected:
	iPhysicsWorld() {}

private:
	iPhysicsWorld(const iPhysicsWorld&) {}
	iPhysicsWorld& operator=(const iPhysicsWorld&) {
		return *this;
	}
};
