#pragma once

#include <vector>

#include <GdpPhysics/interfaces/SphereShape.h>
#include <GdpPhysics/interfaces/PlaneShape.h>

class RigidBody;
struct CollidingBodies
{
	CollidingBodies(RigidBody* a, RigidBody* b) : bodyA(a), bodyB(b) { }
	RigidBody* bodyA;
	RigidBody* bodyB;
};

class CollisionHandler
{
public:
	CollisionHandler();
	~CollisionHandler();

	bool CollideSphereSphere(float dt, RigidBody* bodyA, SphereShape* sphereA,
		RigidBody* bodyB, SphereShape* sphereB);

	bool CollideSpherePlane(float dt, RigidBody* sphere, SphereShape* sphereShape,
		RigidBody* plane, PlaneShape* planeShape);

	glm::vec3 ClosestPtPointPlane(const glm::vec3& pt, const glm::vec3& planeNormal, float planeDotProduct);

	//bool CollideSphereBox(float dt, RigidBody* sphere, SphereShape* sphereShape,
	//	RigidBody* plane, PlaneShape* planeShape);

	void Collide(float dt, std::vector<RigidBody*>& bodies, std::vector<CollidingBodies>& collisions);

};