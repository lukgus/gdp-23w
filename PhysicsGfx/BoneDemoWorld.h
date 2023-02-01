#pragma once

#include "Engine.h"
#include "World.h"

namespace gdp
{

	class BoneDemoWorld : public World
	{
	public:
		BoneDemoWorld();
		~BoneDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;

		virtual void Update(float dt) override;

	private:
		void LoadAssets();

		glm::vec3 m_Bone1Position;
		glm::vec3 m_Bone2Position;
		GameObject* m_BoneObject;
		unsigned int m_ConeModelId;
		unsigned int m_WoodTextureId;
		unsigned int m_WoodMaterialId;
	};
}