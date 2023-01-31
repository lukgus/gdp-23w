#pragma once

#include <GdpPhysics/interfaces/iPhysicsWorld.h>

#include "RigidBody.h"
#include "CollisionHandler.h"

#include <vector>

class PhysicsWorld : public iPhysicsWorld
{
public:
	PhysicsWorld();
	virtual ~PhysicsWorld();

	virtual void SetGravity(const Vector3& gravity) override;

	virtual void AddRigidBody(iRigidBody* body) override;
	virtual void RemoveRigidBody(iRigidBody* body) override;

	virtual void TimeStep(float dt) override;

private:
	Vector3 m_Gravity;

	std::vector<RigidBody*> m_RigidBodies;
	CollisionHandler* m_CollisionHandler;

	PhysicsWorld(const PhysicsWorld&) {}
	PhysicsWorld& operator=(const PhysicsWorld&) {
		return *this;
	}
};
