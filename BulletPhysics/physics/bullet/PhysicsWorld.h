#pragma once

#include <vector>

// Physics Interfaces
#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iCollisionBody.h>
#include <physics/interfaces/iRigidBody.h>

// Bullet Physics
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

namespace physics
{
	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void TimeStep(float dt) override;

		virtual void SetGravity(const glm::vec3& gravity) override;

		virtual void AddBody(iCollisionBody* body) override;
		virtual void RemoveBody(iCollisionBody* body) override;

	private:
		btBroadphaseInterface* m_Broadphase;
		btDefaultCollisionConfiguration* m_CollisionConfiguration;
		btCollisionDispatcher* m_Dispatcher;
		btSequentialImpulseConstraintSolver* m_Solver;
		btDiscreteDynamicsWorld* m_DynamicsWorld;

		typedef std::vector<iCollisionBody*>::iterator body_iterator;
		std::vector<iCollisionBody*> m_Bodies;

		PhysicsWorld(const PhysicsWorld& other) { ; }
		PhysicsWorld& operator=(const PhysicsWorld& other) { return *this; }
	};
}