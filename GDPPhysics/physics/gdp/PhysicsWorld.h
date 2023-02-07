#pragma once

#include <physics/interfaces/iPhysicsWorld.h>

#include "RigidBody.h"
#include "SoftBody.h"
#include "CollisionHandler.h"

#include <vector>

namespace physics
{
	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void SetGravity(const Vector3& gravity) override;

		virtual void AddBody(iCollisionBody* body) override;
		virtual void RemoveBody(iCollisionBody* body) override;

		virtual void TimeStep(float dt) override;

	private:
		Vector3 m_Gravity;

		std::vector<iCollisionBody*> m_Bodies;
		std::vector<RigidBody*> m_RigidBodies;
		std::vector<SoftBody*> m_SoftBodies;

		CollisionHandler* m_CollisionHandler;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) {
			return *this;
		}
	};
}
