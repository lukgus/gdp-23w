#pragma once

#include <vector>

#include <ecs/System.h>

#include <managers/entitymanager.h>
#include <systems/rendersystem.h>

class Engine
{
public:
	Engine();
	~Engine();

	void Initialize();
	void Destroy();

	void Update(float dt);
	void Render();

	unsigned int CreateEntity();

	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		return m_EntityManager.AddComponent<T>(entityId);
	}

	//template <class T>
	//T* AddComponentSlow(unsigned int entityId)
	//{
	//	// Add this to a job queue
	//	return m_EntityManager.AddComponent<T>(entityId);
	//}

	template <class T>
	T* RemoveComponent(unsigned int entityId)
	{
		return m_EntityManager.RemoveComponent<T>(entityId);
	}

	void AddSystem(System* newSystem);
	void RemoveSystem(System* removeSystem);

private:
	EntityManager m_EntityManager;

	//JobQueue m_JobQueue;
	RenderSystem* m_RenderSystem;

	std::vector<System*> m_Systems;
};