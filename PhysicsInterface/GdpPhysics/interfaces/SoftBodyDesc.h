#pragma once

#include <vector>

#include <glm/vec3.hpp>

class SoftBodyDesc
{
public:
	SoftBodyDesc() {}

	std::vector<glm::vec3> NodePositions;
	std::vector<int> TriangulatedIndices;
};