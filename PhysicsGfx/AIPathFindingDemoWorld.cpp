#include "AIPathFindingDemoWorld.h"

#include <assert.h>

//#define DISPLAY_DEBUG_SPHERES 

namespace gdp
{

	void PrintSet(const std::string& name, const std::vector<PathNode*>& set)
	{
		printf("%s { ", name.c_str());

		for (int i = 0; i < set.size(); i++)
		{
			printf("%c ", set[i]->name);
		}

		printf("}\n");
	}

	void Graph::SetNeighbours(int a, int b)
	{
		assert(a >= 0);
		assert(b >= 0);
		assert(a < nodes.size());
		assert(b < nodes.size());

		nodes[a]->neighbours.push_back(nodes[b]);
		nodes[b]->neighbours.push_back(nodes[a]);
	}

	AIPathFindingDemoWorld::AIPathFindingDemoWorld()
	{

	}

	AIPathFindingDemoWorld::~AIPathFindingDemoWorld()
	{

	}

	void AIPathFindingDemoWorld::Initialize()
	{
		LoadAssets();
		CreateGraph();

		//bool pathFound = DepthFirstSearch(&m_Graph, m_Graph.nodes[19], m_Graph.nodes[20]);
		//if (pathFound)
		//{
		//	printf("Found a path between T and U!\n");

		//	PathNode* path = m_Graph.nodes[20];
		//	while (path != nullptr)
		//	{
		//		printf("%c ", path->name);
		//		path = path->parent;
		//	}
		//	printf("\n");
		//}
		//else
		//{
		//	printf("Did not find a path between T and U!\n");
		//}

		m_StartNode = m_Graph.nodes[19];
		m_EndNode = m_Graph.nodes[20];
		m_OpenSet.push_back(m_StartNode);
		printf("Added %c to the open set!\n", m_StartNode->name);
		m_PathNodeGameObjects[m_StartNode->Id]->Renderer.MaterialId = m_OpenMaterialId;
	}

	void AIPathFindingDemoWorld::Destroy()
	{

	}

	void AIPathFindingDemoWorld::Update(float dt)
	{
		SetCameraPosition(glm::vec3(0.f, 0.f, 20.f));
		SetCameraFacingDirection(glm::vec3(0.f, 0.f, -1.f));

		if (GDP_IsKeyPressed('7') && !m_OpenSet.empty())
		{
			PrintSet("open:   ", m_OpenSet);
			PrintSet("closed: ", m_ClosedSet);

			PathNode* X = m_OpenSet.back();
			m_OpenSet.pop_back();
			printf("Removed %c from the open set!\n", X->name);

			if (X == m_EndNode)
			{
				m_OpenSet.clear();

					PathNode* path = m_Graph.nodes[20];
					while (path != nullptr)
					{
						m_PathNodeGameObjects[path->Id]->Renderer.MaterialId = m_FoundPathMaterialId;
						path = path->parent;
					}

				return;
			}

			m_ClosedSet.push_back(X);
			m_PathNodeGameObjects[X->Id]->Renderer.MaterialId = m_ClosedMaterialId;
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(m_OpenSet.begin(), m_OpenSet.end(), X->neighbours[i]) == m_OpenSet.end()
					&& std::find(m_ClosedSet.begin(), m_ClosedSet.end(), X->neighbours[i]) == m_ClosedSet.end())
				{
					m_OpenSet.push_back(n);
					//m_OpenSet.insert(m_OpenSet.begin(), n);
					m_PathNodeGameObjects[n->Id]->Renderer.MaterialId = m_OpenMaterialId;
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}

	}

	void AIPathFindingDemoWorld::LoadAssets()
	{
		// Models
		GDP_LoadModel(m_SphereModelId, "assets/models/uvsphere.fbx");

		// Textures
		GDP_LoadTexture(m_WoodTextureId, "assets/textures/wood.bmp");

		// Create Materials
		GDP_CreateMaterial(m_OpenMaterialId, m_WoodTextureId, glm::vec3(0.0f, 1.f, 0.f));
		GDP_CreateMaterial(m_ClosedMaterialId, m_WoodTextureId, glm::vec3(1.f, 0.f, 0.f));
		GDP_CreateMaterial(m_UnvisitedMaterialId, m_WoodTextureId, glm::vec3(1.f, 1.f, 1.f));
		GDP_CreateMaterial(m_FoundPathMaterialId, m_WoodTextureId, glm::vec3(0.f, 0.f, 1.f));
	}

	void AIPathFindingDemoWorld::CreateGraph()
	{
		CreatePathNode(glm::vec3(0.f, 5.f, 0.f), 'A');
		CreatePathNode(glm::vec3(-4.f, 3.f, 0.f), 'B');
		CreatePathNode(glm::vec3(0.f, 3.f, 0.f), 'C');
		CreatePathNode(glm::vec3(4.f, 3.f, 0.f), 'D');
		CreatePathNode(glm::vec3(-6.f, 1.f, 0.f), 'E');
		CreatePathNode(glm::vec3(-3.f, 1.f, 0.f), 'F');
		CreatePathNode(glm::vec3(-1.f, 1.f, 0.f), 'G');
		CreatePathNode(glm::vec3(1.f, 1.f, 0.f), 'H');
		CreatePathNode(glm::vec3(3.f, 1.f, 0.f), 'I');
		CreatePathNode(glm::vec3(6.f, 1.f, 0.f), 'J');
		CreatePathNode(glm::vec3(-7.f, -1.f, 0.f), 'K');
		CreatePathNode(glm::vec3(-5.f, -1.f, 0.f), 'L');
		CreatePathNode(glm::vec3(-3.f, -1.f, 0.f), 'M');
		CreatePathNode(glm::vec3(-1.f, -1.f, 0.f), 'N');
		CreatePathNode(glm::vec3(1.f, -1.f, 0.f), 'O');
		CreatePathNode(glm::vec3(3.f, -1.f, 0.f), 'P');
		CreatePathNode(glm::vec3(5.f, -1.f, 0.f), 'Q');
		CreatePathNode(glm::vec3(7.f, -1.f, 0.f), 'R');
		CreatePathNode(glm::vec3(-7.f, -3.f, 0.f), 'S');
		CreatePathNode(glm::vec3(-5.f, -3.f, 0.f), 'T');
		CreatePathNode(glm::vec3(3.f, -3.f, 0.f), 'U');

		m_Graph.SetNeighbours(0, 1);
		m_Graph.SetNeighbours(0, 2);
		m_Graph.SetNeighbours(0, 3);
		m_Graph.SetNeighbours(1, 4);
		m_Graph.SetNeighbours(1, 5);
		m_Graph.SetNeighbours(2, 6);
		m_Graph.SetNeighbours(2, 7);
		m_Graph.SetNeighbours(3, 8);
		m_Graph.SetNeighbours(3, 9);
		m_Graph.SetNeighbours(4, 10);
		m_Graph.SetNeighbours(4, 11);
		m_Graph.SetNeighbours(5, 11);
		m_Graph.SetNeighbours(5, 12);
		m_Graph.SetNeighbours(6, 13);
		m_Graph.SetNeighbours(7, 14);
		m_Graph.SetNeighbours(7, 15);
		m_Graph.SetNeighbours(8, 15);
		m_Graph.SetNeighbours(8, 16);
		m_Graph.SetNeighbours(9, 17);
		m_Graph.SetNeighbours(10, 18);
		m_Graph.SetNeighbours(11, 19);
		m_Graph.SetNeighbours(15, 20);
	}

	void AIPathFindingDemoWorld::CreatePathNode(const glm::vec3& position, char name)
	{
		PathNode* newPathNode = new PathNode(name);
		newPathNode->Id = m_Graph.nodes.size();
		m_Graph.nodes.push_back(newPathNode);

		GameObject* ball = GDP_CreateGameObject();
		ball->Position = position;
		ball->Scale = glm::vec3(0.85f);
		ball->Renderer.ShaderId = 1;
		ball->Renderer.MeshId = m_SphereModelId;
		ball->Renderer.MaterialId = m_UnvisitedMaterialId;
		m_PathNodeGameObjects.push_back(ball);
	}
	/*
	*	Depth-First Search
	*	open : set
	*	closed : set
	*	insert start into the open set
	*	while open is not empty
	*		remove the first cell from open; store as X
	*		if X is a goal then return SUCCESS
	*		else
	*			generate children of X
	*			put X in the closed set
	*			discard children of X if already on open or closed
	*			put remaining children on beginning of open set
	*	return FAIL
	*/
	bool AIPathFindingDemoWorld::DepthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		vector<PathNode*> open;
		vector<PathNode*> closed;
		open.push_back(start);
		printf("Added %c to the open set!\n", start->name);
		while (!open.empty())
		{
			PrintSet("open:   ", open);
			PrintSet("closed: ", closed);

			PathNode* X = open.back();
			open.pop_back();
			printf("Removed %c from the open set!\n", X->name);

			if (X == end)
				return true;

			closed.push_back(X);
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(open.begin(), open.end(), X->neighbours[i]) == open.end()
					&& std::find(closed.begin(), closed.end(), X->neighbours[i]) == closed.end())
				{
					open.push_back(n);
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}

		return false;
	}


	/*
	*	Breadth-First Search
	*	open : set
	*	closed : set
	*	insert start into the open set
	*	while open is not empty
	*		remove the first cell from open; store as X
	*		if X is a goal then return SUCCESS
	*		else
	*			generate children of X
	*			put X in the closed set
	*			discard the children of X if already in the open or closed set
	*			put remaining children at the end of the open set
	*	return FAIL
	*/
	bool AIPathFindingDemoWorld::BreadthFirstSearch(Graph* graph, PathNode* start, PathNode* end)
	{
		vector<PathNode*> open;
		vector<PathNode*> closed;
		open.push_back(start);
		printf("Added %c to the open set!\n", start->name);
		while (!open.empty())
		{
			PrintSet("open:   ", open);
			PrintSet("closed: ", closed);

			PathNode* X = open.back();
			open.pop_back();
			printf("Removed %c from the open set!\n", X->name);

			if (X == end)
				return true;

			closed.push_back(X);
			printf("Added %c to the closed set!\n", X->name);

			printf("%c has %d neighbors!\n", X->name, (int)X->neighbours.size());
			for (int i = 0; i < X->neighbours.size(); i++)
			{
				PathNode* n = X->neighbours[i];
				printf("Found neighbour %c!\n", n->name);
				if (std::find(open.begin(), open.end(), X->neighbours[i]) == open.end()
					&& std::find(closed.begin(), closed.end(), X->neighbours[i]) == closed.end())
				{
					open.insert(open.begin(), n);
					n->parent = X;
					printf("Added %c to the open set!\n", n->name);
				}
			}
		}

		return false;
	}

}