#include "pch.h"
#include "PhysicsWorld.h"

#include <iostream>

#include <algorithm>


namespace physics
{
	PhysicsWorld::PhysicsWorld(void)
	{
		printf("PhysX PhysicsWorld!\n");

		m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback,
			m_DefaultErrorCallback);
		if (!m_Foundation)
			throw("PxCreateFoundation failed!");

		m_Pvd = PxCreatePvd(*m_Foundation);

		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		//m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxToleranceScale(), true, m_Pvd);
		m_ToleranceScale.length = 100;
		m_ToleranceScale.speed = 981;
		m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd);
		//m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale);
		physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
		m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		m_Scene = m_Physics->createScene(sceneDesc);

		physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
		if (pvdClient)
		{
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}

		m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
		physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 99), *m_Material);
		m_Scene->addActor(*groundPlane);


	}

	PhysicsWorld::~PhysicsWorld(void)
	{
	}

	void TimeStep(float dt)
	{

	}

	void SetGravity(const glm::vec3& gravity)
	{

	}

	void AddBody(iCollisionBody* body)
	{

	}

	void RemoveBody(iCollisionBody* body)
	{

	}
};
