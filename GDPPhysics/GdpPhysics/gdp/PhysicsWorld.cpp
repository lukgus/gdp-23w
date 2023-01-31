#include "PhysicsWorld.h"

PhysicsWorld::PhysicsWorld()
	: iPhysicsWorld()
	, m_CollisionHandler(nullptr)
{
	m_CollisionHandler = new CollisionHandler();
}

PhysicsWorld::~PhysicsWorld()
{
	// TODO:
	// Clear rigid body vector
	// Delete everything
}

void PhysicsWorld::SetGravity(const Vector3& gravity)
{
	m_Gravity = gravity;
}

void PhysicsWorld::AddRigidBody(iRigidBody* body)
{
	if (body == nullptr)
		return;

	// TODO: Check if body is already in the bodies vector

	// TODO: Check if body is RigidBody type
	m_RigidBodies.push_back(dynamic_cast<RigidBody*>(body));
}

void PhysicsWorld::RemoveRigidBody(iRigidBody* body)
{
	// TODO: Remove body from the vector
}

void PhysicsWorld::TimeStep(float dt)
{
	int todo = 0;
	for (int i = 0; i < m_RigidBodies.size(); i++) {
		m_RigidBodies[i]->Update(dt);
	}

	// Velocity Verlet steps
	// Step #0 Update everything
	for (int i = 0; i < m_RigidBodies.size(); i++) {
		m_RigidBodies[i]->SetGravityAcceleration(m_Gravity);
		m_RigidBodies[i]->UpdateAcceleration();
	}

	// Step #3 : Verlet
	// velocity += acceleration * (dt/2)
	for (int i = 0; i < m_RigidBodies.size(); i++) {
		m_RigidBodies[i]->VerletStep3(dt);
		m_RigidBodies[i]->ApplyDamping(dt / 2.f);
	}

	// Step #1 : Verlet
	// position += ( velocity+acceleration * (dt/2) ) * dt
	for (int i = 0; i < m_RigidBodies.size(); i++) {
		m_RigidBodies[i]->VerletStep1(dt);
	}

	// Handle collisions here
	std::vector<CollidingBodies> collisions;
	m_CollisionHandler->Collide(dt, m_RigidBodies, collisions);

	// A list of collisions..
	// Here we can collision callbacks.


	// Step #2 : Verlet
	// velocity += acceleration * (dt/2)
	for (int i = 0; i < m_RigidBodies.size(); i++) {
		m_RigidBodies[i]->VerletStep2(dt);
		m_RigidBodies[i]->ApplyDamping(dt / 2.f);
		m_RigidBodies[i]->KillForces();

		// Update all listeners for rigid body positions
	}
}