#pragma once

#include "iPhysicsWorld.h"
#include "iRigidBody.h"
#include "RigidBodyDesc.h"
#include "iShape.h"

class iPhysicsFactory
{
public:
	virtual ~iPhysicsFactory() {}

	virtual iPhysicsWorld* CreateWorld() = 0;
	virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) = 0;

protected:
	iPhysicsFactory() {}

private:
	iPhysicsFactory(const iPhysicsFactory&) {}
	iPhysicsFactory& operator=(const iPhysicsFactory&) {
		return *this;
	}
};
