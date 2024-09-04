#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Material.h"
#include "Buffer.h"
#include "Object.h"

struct Mesh
{
	std::string name;
	std::string materialName;

	std::vector<GLuint> indices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
};

struct VertexGroup
{
	int v = -1;
	int t = -1;
	int n = -1;

	bool operator==(const VertexGroup& other) const
	{
		return (v == other.v && t == other.t && n == other.n);
	}
};

typedef std::vector<VertexGroup> Face;

class HashFunction
{
public:
	size_t operator()(const VertexGroup& v) const
	{
		return v.v + v.t + v.n;
	}
};

class Model : public Object
{
public:
	Model();
	bool Load(const std::string& path);
	void Update(float deltaTime) override;
	void Render(const Shader& shader) override;
private:
	void FillBuffers();
	void SortVertexData(Mesh& newMesh, const Mesh& oldMesh, const std::vector<Face>& faces);
	std::vector<Mesh> mMeshes;
	std::vector<class Buffer> mBuffers;
	std::vector<class Material> mMaterials;

	glm::vec3 mDiffuse, mAmbient, mSpecular;
	float mShininess;
};

