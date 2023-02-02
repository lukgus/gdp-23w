#pragma once

#include <ecs/Component.h>

struct MeshRenderer : public Component
{
	unsigned int shaderId;
	unsigned int vbo;
	unsigned int numTriangles;
};