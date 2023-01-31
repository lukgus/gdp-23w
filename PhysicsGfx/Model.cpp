#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace gdp
{
	Assimp::Importer m_AssimpImporter;

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

	Model::Model(const std::vector<glm::vec3>& vertices, const std::vector<int>& triangles)
	{
		unsigned int numVerticesInVertArray = vertices.size();
		unsigned int numIndicesInIndexArray = triangles.size() * 3;
		sVertex_p4t4n4* pTempVertArray = new sVertex_p4t4n4[numIndicesInIndexArray * 2];
		GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];

		unsigned int vertArrayIndex = 0;
		for (unsigned int triIdx = 0; triIdx != triangles.size(); triIdx +=3)
		{
			glm::vec3 a = vertices[triIdx];
			glm::vec3 b = vertices[triIdx + 1];
			glm::vec3 c = vertices[triIdx + 2];

			glm::vec3 ab = a - b;
			glm::vec3 ac = a - c;

			glm::vec3 normal = glm::cross(ab, ac);

			for (int vertIdx = 0; vertIdx != 3; vertIdx++)
			{
				unsigned int index = triIdx + vertIdx;

				glm::vec3 position = vertices[index];
				pTempVertArray[vertArrayIndex].Pos.x = position.x;
				pTempVertArray[vertArrayIndex].Pos.y = position.y;
				pTempVertArray[vertArrayIndex].Pos.z = position.z;
				pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

				pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
				pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;

				pTempVertArray[vertArrayIndex].Normal.x = normal.x;
				pTempVertArray[vertArrayIndex].Normal.y = normal.y;
				pTempVertArray[vertArrayIndex].Normal.z = normal.z;
				pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

				pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

				vertArrayIndex++;
			}
		}

		NumTriangles = triangles.size();

		glGenVertexArrays(1, &Vbo);
		glBindVertexArray(Vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &VertexBufferId);
		glGenBuffers(1, &IndexBufferId);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

		unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4); ;
		glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

		unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
		unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);
		unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4, TexUVx2);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);

		unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

		glBindVertexArray(0);

		delete[] pTempVertArray;
		delete[] pIndexArrayLocal;
	}

	Model::Model(const char* filepath) {
		const aiScene* scene = m_AssimpImporter.ReadFile(filepath,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_PopulateArmatureData |
			aiProcess_FixInfacingNormals |
			aiProcess_LimitBoneWeights);

		aiMesh* mesh = scene->mMeshes[0];

		if (scene == 0 || !scene->HasMeshes())
			return;

		unsigned int numVerticesInVertArray = mesh->mNumVertices;
		unsigned int numIndicesInIndexArray = mesh->mNumFaces * 3;
		sVertex_p4t4n4* pTempVertArray = new sVertex_p4t4n4[numIndicesInIndexArray * 2];
		GLuint* pIndexArrayLocal = new GLuint[numIndicesInIndexArray * 2];
		int count = 0;
		int index = 0;

		for (unsigned int vertIdx = 0; vertIdx != mesh->mNumVertices; vertIdx++)
		{
			aiVector3D position = mesh->mVertices[vertIdx];
			vertices.push_back(glm::vec3(position.x, position.y, position.z));
		}

		unsigned int vertArrayIndex = 0;
		for (unsigned int faceIdx = 0; faceIdx != mesh->mNumFaces; faceIdx++)
		{
			aiFace face = mesh->mFaces[faceIdx];

			for (int idx = 0; idx != 3; idx++)
			{
				unsigned int index = face.mIndices[idx];

				triangles.push_back(index);

				aiVector3D position = mesh->mVertices[index];
				pTempVertArray[vertArrayIndex].Pos.x = position.x;
				pTempVertArray[vertArrayIndex].Pos.y = position.y;
				pTempVertArray[vertArrayIndex].Pos.z = position.z;
				pTempVertArray[vertArrayIndex].Pos.w = 1.0f;

				if (mesh->HasTextureCoords(0)) {
					aiVector3D textureCoord = mesh->mTextureCoords[0][index];
					pTempVertArray[vertArrayIndex].TexUVx2.x = textureCoord.x;
					pTempVertArray[vertArrayIndex].TexUVx2.y = textureCoord.y;
					pTempVertArray[vertArrayIndex].TexUVx2.z = textureCoord.z;
					pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
				}
				else
				{
					pTempVertArray[vertArrayIndex].TexUVx2.x = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.y = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.z = 0.0f;
					pTempVertArray[vertArrayIndex].TexUVx2.w = 0.0f;
				}

				aiVector3D normal = mesh->mNormals[index];
				pTempVertArray[vertArrayIndex].Normal.x = normal.x;
				pTempVertArray[vertArrayIndex].Normal.y = normal.y;
				pTempVertArray[vertArrayIndex].Normal.z = normal.z;
				pTempVertArray[vertArrayIndex].Normal.w = 0.0f;

				pIndexArrayLocal[vertArrayIndex] = vertArrayIndex;

				vertArrayIndex++;
			}
		}

		NumTriangles = mesh->mNumFaces;

		glGenVertexArrays(1, &Vbo);
		glBindVertexArray(Vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &VertexBufferId);
		glGenBuffers(1, &IndexBufferId);

		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);

		unsigned int totalVertBufferSizeBYTES = numIndicesInIndexArray * sizeof(sVertex_p4t4n4); ;
		glBufferData(GL_ARRAY_BUFFER, totalVertBufferSizeBYTES, pTempVertArray, GL_STATIC_DRAW);

		unsigned int bytesInOneVertex = sizeof(sVertex_p4t4n4);
		unsigned int byteOffsetToPosition = offsetof(sVertex_p4t4n4, Pos);
		unsigned int byteOffsetToNormal = offsetof(sVertex_p4t4n4, Normal);
		unsigned int byteOffsetToUVCoords = offsetof(sVertex_p4t4n4, TexUVx2);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToPosition);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToUVCoords);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, bytesInOneVertex, (GLvoid*)byteOffsetToNormal);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);

		unsigned int sizeOfIndexArrayInBytes = numIndicesInIndexArray * sizeof(GLuint);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndexArrayInBytes, pIndexArrayLocal, GL_STATIC_DRAW);

		glBindVertexArray(0);

		delete[] pTempVertArray;
		delete[] pIndexArrayLocal;
	}

	Model::~Model() {

	}
}