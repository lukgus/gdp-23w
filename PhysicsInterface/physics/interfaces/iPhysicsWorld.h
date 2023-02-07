#pragma once

#include "Math.h"
#include "iCollisionBody.h"

namespace physics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void SetGravity(const Vector3& gravity) = 0;

		virtual void AddBody(iCollisionBody* body) = 0;
		virtual void RemoveBody(iCollisionBody* body) = 0;

		virtual void TimeStep(float dt) = 0;

	protected:
		iPhysicsWorld() {}

	private:
		iPhysicsWorld(const iPhysicsWorld&) {}
		iPhysicsWorld& operator=(const iPhysicsWorld&) {
			return *this;
		}
	};
}