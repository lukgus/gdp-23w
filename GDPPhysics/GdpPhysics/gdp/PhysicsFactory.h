#pragma once

#include <GdpPhysics/interfaces/iPhysicsFactory.h>

class PhysicsFactory : public iPhysicsFactory
{
public:
	PhysicsFactory();
	virtual ~PhysicsFactory();

	virtual iPhysicsWorld* CreateWorld() override;
	virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) override;
};
