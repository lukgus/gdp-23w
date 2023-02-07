#include "PhysicsWorld.h"

#include <iostream>

#include <algorithm>

#include "Conversion.h"

#include "RigidBody.h"

namespace physics

{
	PhysicsWorld::PhysicsWorld(void)
	{
		m_Broadphase = new btDbvtBroadphase();				// not sure the main reason or difference

		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);

		m_Solver = new btSequentialImpulseConstraintSolver;

		m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfiguration);
	}

	PhysicsWorld::~PhysicsWorld(void)
	{
		// Remove rigid bodies from dynamics world
		// delete motion states
		// delete rigid bodies
		// Delete all of the shapes

		delete m_DynamicsWorld;
		delete m_Solver;
		delete m_CollisionConfiguration;
		delete m_Dispatcher;
		delete m_Broadphase;
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
		m_DynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		m_DynamicsWorld->stepSimulation(dt, 10);
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		if (body->GetBodyType() != BodyType::RigidBody)
			return;

		auto findBodyIt = std::find(m_Bodies.begin(), m_Bodies.end(), body);
		if (findBodyIt != m_Bodies.end())
		{
			// Body is already added, just return
			return;
		}
		m_Bodies.push_back(body);
		m_DynamicsWorld->addRigidBody(CastBulletRigidBody(body));
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		if (body->GetBodyType() != BodyType::RigidBody)
			return;

		auto findBodyIt = std::find(m_Bodies.begin(), m_Bodies.end(), body);
		if (findBodyIt == m_Bodies.end())
		{
			// Body is not added, just return
			return;
		}

		m_Bodies.erase(findBodyIt);
		m_DynamicsWorld->removeRigidBody(CastBulletRigidBody(body));
	}
};
