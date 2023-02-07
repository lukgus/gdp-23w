#pragma once

#include "World.h"
#include "Engine.h"

#include <queue>
#include <vector>
using namespace std;

namespace gdp
{
	struct PathNode
	{
		PathNode(char name)
			: name(name)
			, parent(nullptr)
		{
		}

		PathNode* parent;
		int Id;
		char name;
		vector<PathNode*> neighbours;
	private:
		PathNode() : name(' '){}
	};

	struct Graph
	{
		//PathNode* AddNode();
		//void SetNeighbours(PathNode* a, PathNode* b);
		void SetNeighbours(int a, int b);

		vector<PathNode*> nodes;
	};

	class AIPathFindingDemoWorld : public World
	{
	public:
		AIPathFindingDemoWorld();
		~AIPathFindingDemoWorld();

		virtual void Initialize() override;
		virtual void Destroy() override;
		virtual void Update(float dt) override;

	private:
		void LoadAssets();
		void CreateGraph();
		void CreatePathNode(const glm::vec3& position, char name);

		bool BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end);
		bool DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end);

		unsigned int m_SphereModelId;
		unsigned int m_WoodTextureId;

		unsigned int m_OpenMaterialId;
		unsigned int m_ClosedMaterialId;
		unsigned int m_UnvisitedMaterialId;
		unsigned int m_FoundPathMaterialId;

		vector<PathNode*> m_OpenSet;
		vector<PathNode*> m_ClosedSet;
		PathNode* m_StartNode;
		PathNode* m_EndNode;

		// Create a Graph and nodes
		Graph m_Graph;

		std::vector<GameObject*> m_PathNodeGameObjects;
	};
}