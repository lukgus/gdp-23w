#include "RigidBody.h"

#include "Conversion.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

namespace physics
{
	RigidBody* RigidBody::Cast(iCollisionBody* body)
	{
		return dynamic_cast<RigidBody*>(body);
	}

	RigidBody::RigidBody(const RigidBodyDesc& desc, iShape* shape)
	{
		btQuaternion ori;
		btVector3 pos;

		CastBulletQuaternion(desc.rotation, &ori);
		CastBulletVector3(desc.position, &pos);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(ori, pos));
		btVector3 inertia(0, 0, 0);
		btCollisionShape* bulletShape = CastBulletShape(shape);

		if (desc.mass != 0.0f)
		{
			bulletShape->calculateLocalInertia(desc.mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo bodyCI(desc.mass, motionState, bulletShape, inertia);

		m_BulletBody = new btRigidBody(bodyCI);

		btVector3 btAngularFactor;
		btVector3 btLinearFactor;

		CastBulletVector3(desc.angularFactor, &btAngularFactor);
		CastBulletVector3(desc.linearFactor, &btLinearFactor);

		m_BulletBody->setAngularFactor(btAngularFactor);
		m_BulletBody->setLinearFactor(btLinearFactor);

		return;
	}

	RigidBody::~RigidBody(void)
	{
	}

	bool RigidBody::IsStatic(void) const
	{
		return m_BulletBody->isStaticObject();
	}

	iShape* RigidBody::GetShape(void)
	{
		return NULL;
	}

	//void RigidBody::GetWorldTransform(glm::mat4& transformOut)
	//{
	//}

	void RigidBody::GetPosition(glm::vec3& positionOut)
	{
		CastGLMVec3(m_BulletBody->getCenterOfMassPosition(), &positionOut);
	}

	void RigidBody::SetPosition(const glm::vec3& positionIn)										   
	{
		// Can't.
	}

	void RigidBody::GetRotation(glm::quat& orientationOut)
	{
		CastGLMQuat(m_BulletBody->getOrientation(), &orientationOut);
	}

	void RigidBody::SetRotation(const glm::quat& orientationIn)
	{
		// Can't
	}

	void RigidBody::ApplyForce(const glm::vec3& force)
	{
		btVector3 btForce;
		CastBulletVector3(force, &btForce);
		m_BulletBody->applyCentralForce(btForce);
	}

	void RigidBody::ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint)
	{
		btVector3 btForce;
		btVector3 btForceAtPoint;

		CastBulletVector3(force, &btForce);
		CastBulletVector3(relativePoint, &btForceAtPoint);

		m_BulletBody->applyForce(btForce, btForceAtPoint);
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		btVector3 btImpulse;

		CastBulletVector3(impulse, &btImpulse);

		m_BulletBody->applyCentralImpulse(btImpulse);
	}

	void RigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint)
	{
		btVector3 btImpulse;
		btVector3 btImpulseAtPoint;

		CastBulletVector3(impulse, &btImpulse);
		CastBulletVector3(relativePoint, &btImpulseAtPoint);

		m_BulletBody->applyImpulse(btImpulse, btImpulseAtPoint);
	}

	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
		btVector3 btTorque;

		CastBulletVector3(torque, &btTorque);

		m_BulletBody->applyTorque(btTorque);
	}

	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
	{
		btVector3 btTorqueImpulse;

		CastBulletVector3(torqueImpulse, &btTorqueImpulse);

		m_BulletBody->applyTorqueImpulse(btTorqueImpulse);
	}

};