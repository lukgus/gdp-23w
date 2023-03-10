#include "Engine.h"
#include "World.h"
#include "PhysicsDemoWorld.h"
#include "AIDemoWorld.h"
#include "AIPathFindingDemoWorld.h"
#include "AnimationDemoWorld.h"
#include "PhysicsDemoWorld.h"
#include "BoneDemoWorld.h"
#include "CharacterAnimationDemoWorld.h"

#include <string>

using namespace gdp;

World* g_World;
void Update(float dt)
{
	g_World->Update(dt);
}

void LoadWorld(const std::string& worldName)
{
	if (strcmp(worldName.c_str(), "PhysicsDemoWorld") == 0)
	{
		g_World = new PhysicsDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AIDemoWorld") == 0)
	{
		g_World = new AIDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AIPathFindingDemoWorld") == 0)
	{
		g_World = new AIPathFindingDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "AnimationDemoWorld") == 0)
	{
		g_World = new AnimationDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "BoneDemoWorld") == 0)
	{
		g_World = new BoneDemoWorld();
	}
	else if (strcmp(worldName.c_str(), "CharacterAnimationDemoWorld") == 0)
	{
		g_World = new CharacterAnimationDemoWorld();
	}
}

void InitializeWorld()
{
	g_World->Initialize();
}


int main(int argc, char** argv)
{
	if (argc != 3)
		LoadWorld("PhysicsDemoWorld");
	else
		LoadWorld(argv[2]);

	GDP_Initialize();
	GDP_CreateWindow("Test", 1200, 800);
	GDP_UpdateCallback(&Update);

	InitializeWorld();
	GDP_Run();

	g_World->Destroy();
	GDP_Destroy();
	return 0;
}