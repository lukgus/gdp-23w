#pragma once

#include "GL.h"

#include <vector>

namespace gdp
{
	typedef struct sFloat4 {
		float x, y, z, w;
	} sFloat4;

	typedef struct sUint4 {
		unsigned int x, y, z, w;
	} sUint4;

	typedef struct sInt4 {
		int x, y, z, w;
	} sInt4;

	typedef struct sVertex_p4t4n4 {
		sFloat4 Pos;
		sFloat4 TexUVx2;
		sFloat4 Normal;
	} sVertex_p4t4n4;

	typedef struct sVertex_p4t4n4b4 {
		sFloat4 Pos;
		sFloat4 TexUVx2;
		sFloat4 Normal;
		sFloat4 BoneIds;
	} sVertex_p4t4n4b4;

	class Model {
	public:
		Model(const char* filepath);
		Model(const char* filepath, bool withBones);
		Model(const std::vector<glm::vec3>& vertices, const std::vector<int>& triangles);
		~Model();

		GLuint Vbo;
		GLuint VertexBufferId;
		GLuint IndexBufferId;

		std::vector<glm::vec3> vertices;
		std::vector<int> triangles;	// 1,2,3

		unsigned int NumTriangles;
	};
}