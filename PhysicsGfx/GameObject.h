#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "MeshRenderer.h"
#include "Animation.h"
#include <GdpPhysics/interfaces/iCollisionBody.h>

namespace gdp
{
	class GameObject {
	public:
		GameObject()
			: Position(0.0f)
			, Velocity(0.0f)
			, Steering(0.0f)
			, Scale(1.0f)
			, Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
			, Renderer()
			, PhysicsBody(nullptr)
			, Enabled(true)
			, HasParent(false)
		{ }

		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;

		// Artificial Intelligence
		glm::vec3 Velocity;
		glm::vec3 Steering;

		MeshRenderer Renderer;
		bool Enabled;

		// Physics & Simulation
		iCollisionBody* PhysicsBody;

		// Animation
		bool HasParent;
		bool HasBones;
		glm::mat4 BoneModelMatrices[4];
		Animation Animation;
		std::vector<GameObject*> Children;
	};
}