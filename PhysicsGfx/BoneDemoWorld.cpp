#include "BoneDemoWorld.h"
#include "Animation.h"

namespace gdp
{
	BoneDemoWorld::BoneDemoWorld()
	{

	}

	BoneDemoWorld::~BoneDemoWorld()
	{

	}

	void BoneDemoWorld::Initialize()
	{
		LoadAssets();

		m_BoneObject = GDP_CreateGameObject();
		m_BoneObject->Position = glm::vec3(0.f);
		m_BoneObject->HasBones = true;
		m_BoneObject->Renderer.MeshId = m_ConeModelId;
		m_BoneObject->Renderer.MaterialId = m_WoodMaterialId;
		m_BoneObject->BoneModelMatrices[0] = glm::mat4(1.f);	// identity matrix
	}

	void BoneDemoWorld::Destroy()
	{
	}

	void BoneDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 0.f, 10.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		if (GDP_IsKeyHeldDown('w'))
		{
			m_Bone1Position.y += dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('s'))
		{
			m_Bone1Position.y -= dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('a'))
		{
			m_Bone1Position.x -= dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('d'))
		{
			m_Bone1Position.x += dt * 10.f;
		}

		if (GDP_IsKeyHeldDown('t'))
		{
			m_Bone2Position.y += dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('g'))
		{
			m_Bone2Position.y -= dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('f'))
		{
			m_Bone2Position.x -= dt * 10.f;
		}
		if (GDP_IsKeyHeldDown('h'))
		{
			m_Bone2Position.x += dt * 10.f;
		}

		m_BoneObject->BoneModelMatrices[0] = glm::translate(glm::mat4(1.0f), m_Bone1Position);

		m_BoneObject->BoneModelMatrices[1] = glm::translate(m_BoneObject->BoneModelMatrices[0], m_Bone2Position);
		m_BoneObject->BoneModelMatrices[2] = glm::translate(m_BoneObject->BoneModelMatrices[1], glm::vec3(0.f));
		m_BoneObject->BoneModelMatrices[3] = glm::translate(m_BoneObject->BoneModelMatrices[2], glm::vec3(0.f));

		//glm::mat4 RotationMatrix = glm::mat4_cast(go->Rotation);
		//glm::mat4 ScaleMatrix = glm::scale(glm::mat4(1.0f), go->Scale);
		//glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix;
	}

	void BoneDemoWorld::LoadAssets()
	{
		GDP_LoadModel(m_ConeModelId, "assets/models/scarytentacle.obj", true);

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}
}