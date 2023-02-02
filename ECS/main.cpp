#include <engine/engine.h>
#include <systems/motionsystem.h>
#include <components/positioncomponent.h>
#include <components/velocitycomponent.h>

Engine g_Engine;

unsigned int CreateAnEntity()
{
	unsigned int entity = g_Engine.CreateEntity();
	PositionComponent* posCmp = g_Engine.AddComponent<PositionComponent>(entity);

	posCmp->x = 0;
	posCmp->y = 0;
	posCmp->z = 0;

	return entity;
}

struct PositionVelocityEntityType
{
	PositionComponent position;
	VelocityComponent velocity;
};

std::vector<PositionVelocityEntityType> position_velocity_types;

int main(int argc, char** argv)
{
	MotionSystem motion;
	g_Engine.AddSystem(&motion);

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);

	unsigned int entity = CreateAnEntity();

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);


	VelocityComponent* velCmp = g_Engine.AddComponent<VelocityComponent>(entity);
	velCmp->vx = 10;
	velCmp->vy = 0;
	velCmp->vz = 0;

	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);
	g_Engine.Update(0.1f);

	g_Engine.RemoveSystem(&motion);

	return 0;
}
