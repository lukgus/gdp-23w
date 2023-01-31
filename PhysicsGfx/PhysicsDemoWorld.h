#pragma once

#include "Engine.h"
#include "World.h"

namespace gdp
{
	class PhysicsDemoWorld : public World
	{
	public:
		PhysicsDemoWorld();
		~PhysicsDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void CreateGround();
		void CreateBall(const glm::vec3& position, float size);
		void CreatePlayerBall(const glm::vec3& position);

		unsigned int m_PlaneModelId;
		unsigned int m_SphereModelId;

		unsigned int m_WoodTextureId;
		unsigned int m_BlankTextureId;

		unsigned int m_GroundMaterialId;
		unsigned int m_RedBallMaterialId;
		unsigned int m_BlueBallMaterialId;

		GameObject* m_GroundGameObject;
		GameObject* m_ControllableBall;
		std::vector<GameObject*> m_BallList;
	};
}