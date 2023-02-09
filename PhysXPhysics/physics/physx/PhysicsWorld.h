#pragma once
#include <vector>

// Physics Interfaces
#include <physics/interfaces/iPhysicsWorld.h>
#include <physics/interfaces/iCollisionBody.h>
#include <physics/interfaces/iRigidBody.h>

#include <glm/vec3.hpp>

#include <PxPhysicsAPI.h>

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
		physx::PxDefaultAllocator m_DefaultAllocatorCallback;
		physx::PxDefaultErrorCallback m_DefaultErrorCallback;
		physx::PxFoundation* m_Foundation = nullptr;
		physx::PxPhysics* m_Physics = nullptr;
		physx::PxDefaultCpuDispatcher* m_Dispatcher = nullptr;
		physx::PxScene* m_Scene = nullptr;
		physx::PxMaterial* m_Material = nullptr;
		physx::PxPvd* m_Pvd = nullptr;
		physx::PxTolerancesScale m_ToleranceScale;

		PhysicsWorld(const PhysicsWorld& other) { ; }
		PhysicsWorld& operator=(const PhysicsWorld& other) { return *this; }
	};
}