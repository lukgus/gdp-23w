#include "CharacterAnimationDemoWorld.h"
#include "Animation.h"

namespace gdp
{
	CharacterAnimationDemoWorld::CharacterAnimationDemoWorld()
	{

	}

	CharacterAnimationDemoWorld::~CharacterAnimationDemoWorld()
	{

	}

	void CharacterAnimationDemoWorld::Initialize()
	{
		LoadAssets();
		LoadAnimations();


		GameObject* character = GDP_CreateGameObject();
		character->Animation.AnimationTime = 0.f;
		character->Animation.IsLooping = true;
		character->Animation.IsPlaying = true;
		character->Animation.AnimationType = m_AnimationName;
		character->Animation.Speed = 1.f;
		character->Scale *= 0.1f;
		character->Position = glm::vec3(0.f, 0.f, 0.f);
		character->Renderer.MeshId = m_CharacterModelId;
		character->Renderer.MaterialId = m_WoodMaterialId;
		character->HasBones = true;
	}

	void CharacterAnimationDemoWorld::Destroy()
	{
		if (m_Character != nullptr)
			delete m_Character;
	}

	void CharacterAnimationDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 5.f, 30.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));
	}

	void CharacterAnimationDemoWorld::LoadAssets()
	{
		// Model
		GDP_LoadFBXFile(m_CharacterModelId, m_AnimationName, "assets/models/neutral.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_WoodMaterialId, m_WoodTextureId, glm::vec3(1.0f));
	}

	void CharacterAnimationDemoWorld::LoadAnimations()
	{
	}
}