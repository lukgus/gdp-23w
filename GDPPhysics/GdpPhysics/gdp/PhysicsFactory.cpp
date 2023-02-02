#include "PhysicsFactory.h"

#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "SoftBody.h"

PhysicsFactory::PhysicsFactory()
	: iPhysicsFactory()
{ }

PhysicsFactory::~PhysicsFactory()
{ }

iPhysicsWorld* PhysicsFactory::CreateWorld()
{
	return new PhysicsWorld();
}

iRigidBody* PhysicsFactory::CreateRigidBody(const RigidBodyDesc& desc, iShape* shape)
{
	return new RigidBody(desc, shape);
}

iSoftBody* PhysicsFactory::CreateSoftBody(const SoftBodyDesc& desc)
{
	return new SoftBody(desc);
}