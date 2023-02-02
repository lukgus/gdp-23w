#include "PhysicsDemoWorld.h"

namespace gdp
{
	PhysicsDemoWorld::PhysicsDemoWorld()
	{

	}

	PhysicsDemoWorld::~PhysicsDemoWorld()
	{

	}

	void PhysicsDemoWorld::Initialize()
	{
		LoadAssets();
		CreateGround();

		CreateBall(glm::vec3(-5, 10, -5), 0.1f);
		CreateBall(glm::vec3(-5, 4, 5), 5.f);
		CreateBall(glm::vec3(5, 6, 5), 10.f);
		CreateBall(glm::vec3(5, 3, -5), 20.f);

		CreatePlayerBall(glm::vec3(0, 3, 0));

		GDP_GetPhysicsWorld()->SetGravity(glm::vec3(0, -0.0981, 0));
	}

	void PhysicsDemoWorld::Destroy()
	{

	}

	void PhysicsDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0, 12, 36));
		SetCameraFacingDirection(glm::vec3(0, -.2, -0.8));

		iRigidBody* rigidBody = m_ControllableBall->RigidBody;
		if (rigidBody != nullptr)
		{
			float force = 0.25;
			glm::vec3 direction(0.f);


			if (GDP_IsKeyHeldDown('w'))
			{
				direction.z += -1;
			}
			if (GDP_IsKeyHeldDown('a'))
			{
				direction.x += -1;
			}
			if (GDP_IsKeyHeldDown('s'))
			{
				direction.z += 1;
			}
			if (GDP_IsKeyHeldDown('d'))
			{
				direction.x += 1;
			}
			if (GDP_IsKeyPressed(' '))
			{
				rigidBody->ApplyImpulse(glm::vec3(0.f, 0.5f, 0.f));
			}

			//rigidBody->ApplyTorque(direction * force);
			rigidBody->ApplyForce(direction * force);
			//rigidBody->ApplyForceAtPoint(direction * force, glm::vec3(0.f, 1.f, 0.f));

			// This should be updated right after physics update step.
			m_ControllableBall->RigidBody->GetPosition(m_ControllableBall->Position);
			m_ControllableBall->RigidBody->GetRotation(m_ControllableBall->Rotation);
		}

		for (int i = 0; i < m_BallList.size(); i++)
		{
			GameObject* ball = m_BallList[i];
			if (ball->RigidBody != nullptr)
			{
				glm::vec3 position;

				ball->RigidBody->GetPosition(position);
				ball->RigidBody->GetRotation(ball->Rotation);

				ball->Position = position;

				if (position.x < -16.f)
				{
					ball->RigidBody->ApplyForce(glm::vec3(1.f, 0.f, 0.f));
				}

				if (position.x > 16.f)
				{
					ball->RigidBody->ApplyForce(glm::vec3(-1.f, 0.f, 0.f));
				}

				if (position.z < -16.f)
				{
					ball->RigidBody->ApplyForce(glm::vec3(0.f, 0.f, 1.f));
				}

				if (position.z > 16.f)
				{
					ball->RigidBody->ApplyForce(glm::vec3(0.f, 0.f, -1.f));
				}

			}
		}

	}

	void PhysicsDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_PlaneModelId, "assets/models/plane.fbx");
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");
		GDP_LoadTexture(m_BlankTextureId, "assets/textures/MetalPipeWallRusty_opacity.png");

		// Create Materials
		GDP_CreateMaterial(m_GroundMaterialId, m_WoodTextureId, glm::vec3(1.0f));
		GDP_CreateMaterial(m_RedBallMaterialId, m_WoodTextureId, glm::vec3(1.0f, 0.8f, 0.8f));
		GDP_CreateMaterial(m_BlueBallMaterialId, m_WoodTextureId, glm::vec3(0.8f, 0.8f, 1.0f));
	}

	void PhysicsDemoWorld::CreateGround()
	{
		GameObject* ground = GDP_CreateGameObject();
		ground->Position = glm::vec3(0, 0, 0);
		ground->Scale = glm::vec3(16, 16, 1);
		ground->Rotation = glm::angleAxis(glm::radians(-90.f), glm::vec3(1, 0, 0));
		ground->Renderer.ShaderId = 1;
		ground->Renderer.MeshId = m_PlaneModelId;
		ground->Renderer.MaterialId = m_GroundMaterialId;

		iShape* planeShape = new PlaneShape(0.0f, glm::vec3(0.f, 1.f, 0.f));
		RigidBodyDesc desc;
		desc.isStatic = true;
		desc.mass = 0;
		desc.position = glm::vec3(0.f);
		desc.linearVelocity = glm::vec3(0.f);
		ground->RigidBody = GDP_CreateRigidBody(desc, planeShape);
		GDP_GetPhysicsWorld()->AddBody(ground->RigidBody);
	}

	void PhysicsDemoWorld::CreateBall(const glm::vec3& position, float size)
	{
		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Scale = glm::vec3(1.f);
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_RedBallMaterialId;

		iShape* ballShape = new SphereShape(1.0f);
		RigidBodyDesc desc;
		desc.isStatic = false;
		desc.mass = size;
		desc.position = position;
		desc.linearVelocity = glm::vec3(0.f);
		ball->RigidBody = GDP_CreateRigidBody(desc, ballShape);

		m_BallList.push_back(ball);
		GDP_GetPhysicsWorld()->AddBody(ball->RigidBody);
	}

	void PhysicsDemoWorld::CreatePlayerBall(const glm::vec3& position)
	{
		m_ControllableBall = GDP_CreateGameObject();
		m_ControllableBall->Position = position;
		m_ControllableBall->Renderer.ShaderId = 1;
		m_ControllableBall->Renderer.MeshId = m_SphereModelId;
		m_ControllableBall->Renderer.MaterialId = m_BlueBallMaterialId;

		iShape* ballShape = new SphereShape(1.0f);
		RigidBodyDesc desc;
		desc.isStatic = false;
		desc.mass = 1.f;
		desc.position = position;
		desc.linearVelocity = glm::vec3(0.f);
		m_ControllableBall->RigidBody = GDP_CreateRigidBody(desc, ballShape);

		GDP_GetPhysicsWorld()->AddBody(m_ControllableBall->RigidBody);
	}
}