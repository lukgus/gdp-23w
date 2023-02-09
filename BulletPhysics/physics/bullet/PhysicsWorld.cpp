#include "PhysicsWorld.h"
#include "Conversion.h"

namespace physics
{
	PhysicsWorld::PhysicsWorld()
		: iPhysicsWorld()
	{
		m_CollisionConfiguration = new btDefaultCollisionConfiguration();
		m_Dispatcher = new btCollisionDispatcher(m_CollisionConfiguration);
		m_OverlappingPairCache = new btDbvtBroadphase();
		m_Solver = new btSequentialImpulseConstraintSolver;
		m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, 
			m_OverlappingPairCache, m_Solver, m_CollisionConfiguration);
	}

	PhysicsWorld::~PhysicsWorld()
	{
		delete m_DynamicsWorld;
		delete m_CollisionConfiguration;
		delete m_Dispatcher;
		delete m_OverlappingPairCache;
		delete m_Solver;
	}

	void PhysicsWorld::SetGravity(const Vector3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
		m_DynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		m_DynamicsWorld->addRigidBody(bulletBody);
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		m_DynamicsWorld->removeRigidBody(bulletBody);
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		m_DynamicsWorld->stepSimulation(dt);
	}
}
