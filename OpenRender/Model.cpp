#include <fstream>
#include <cstdio>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.h"
#include "Shader.h"
#include "Utility.h"
#include "Input.h"

Model::Model()
{
	// Anticipate at least 10 meshes and buffers
	mMeshes.reserve(10);
	mBuffers.reserve(10);

	mAmbient = glm::vec3(0.4f, 0.4f, 0.4f);
	mDiffuse = glm::vec3(0.1f, 0.7f, 0.2f);
	mSpecular = glm::vec3(0.8f, 0.8f, 0.8f);
	mShininess = 50.f;
}

bool Model::Load(const std::string& path)
{
	std::ifstream iFile(path, std::ios_base::in);
	if (!iFile)
	{
		printf("Could not find .obj file at path: %s\n", path.c_str());
		return false;
	}

	Mesh rawMesh;
	std::string lastMaterialName;

	std::string line;
	std::string lastName;
	std::vector<Face> faces;

	std::vector<std::string> substrings;
	substrings.reserve(10);

	while (!iFile.eof())
	{
		std::getline(iFile, line);
		substrings.clear();
		if (!line.empty() && line[0] != '#')
		{
			Utility::ParseString(line, substrings, ' ');

			// Vertex Data
			if (substrings[0] == "v")
			{
				rawMesh.vertices.push_back(glm::vec3(std::stof(substrings[1]), std::stof(substrings[2]), std::stof(substrings[3])));
				continue;
			}

			// Normal Data
			if (substrings[0] == "vn")
			{
				rawMesh.normals.push_back(glm::vec3(std::stof(substrings[1]), std::stof(substrings[2]), std::stof(substrings[3])));
				continue;
			}

			// Texture Coordinate Data
			if (substrings[0] == "vt")
			{
				rawMesh.texCoords.push_back(glm::vec2(std::stof(substrings[1]), std::stof(substrings[2])));
				continue;
			}

			// Faces
			if (substrings[0] == "f")
			{
				Face face;
				std::vector<std::string> numbers;
				numbers.reserve(10);
				
				for (unsigned int i = 1; i <= 3; i++)
				{
					numbers.clear();
					VertexGroup vertexGroup;
					Utility::ParseString(substrings[i], numbers, '/');

					// .obj file is 1 indexed, subtract 1 to convert to 0 indexed
					vertexGroup.v = std::stoi(numbers[0]) - 1;
					vertexGroup.t = std::stoi(numbers[1]) - 1;
					vertexGroup.n = std::stoi(numbers[2]) - 1;

					face.push_back(vertexGroup);
				}

				faces.push_back(face);

				continue;
			}

			// Material group
			if (substrings[0] == "usemtl")
			{
				if (!mMaterials.empty())
				{
					for (const auto& material : mMaterials)
					{
						if (material.GetName() == substrings[1])
						{
							lastMaterialName = substrings[1];
							break;
						}
					}
				}
				continue;
			}

			// Material file
			if (substrings[0] == "mtllib")
			{
				Material material;
				material.Load(substrings[1], mMaterials);
				continue;
			}

			// Group Data
			if (substrings[0] == "g" || substrings[0] == "o")
			{
				if (!faces.empty())
				{
					Mesh mesh;
					mesh.name = lastName;
					mesh.materialName = lastMaterialName;
					SortVertexData(mesh, rawMesh, faces);
					mMeshes.push_back(mesh);
				}

				lastName = substrings[1];
				faces.clear();
				continue;
			}
		}
	}
	iFile.close();
	if (!faces.empty())
	{
		Mesh mesh;
		mesh.name = lastName;
		mesh.materialName = lastMaterialName;
		SortVertexData(mesh, rawMesh, faces);
		mMeshes.push_back(mesh);
	}

	FillBuffers();
	return true;
}

void Model::FillBuffers()
{
	for (auto& mesh : mMeshes)
	{
		Buffer buffer;
		buffer.CreateBuffer(mesh.indices.size(), true);

		buffer.FillEBO(&mesh.indices[0], mesh.indices.size() * sizeof(GLuint), Buffer::FillType::Once);
		buffer.FillVBO(Buffer::VBOType::VertexBuffer, &mesh.vertices[0].x, mesh.vertices.size() * sizeof(glm::vec3), Buffer::FillType::Once);
		buffer.FillVBO(Buffer::VBOType::TextureBuffer, &mesh.texCoords[0].x, mesh.texCoords.size() * sizeof(glm::vec2), Buffer::FillType::Once);
		buffer.FillVBO(Buffer::VBOType::NormalBuffer, &mesh.normals[0].x, mesh.normals.size() * sizeof(glm::vec3), Buffer::FillType::Once);

		for (const auto& vertex : mesh.vertices)
		{
			glm::vec4 color(1.0f);
			mesh.colors.push_back(color);
		}
		buffer.FillVBO(Buffer::VBOType::ColorBuffer, &mesh.colors[0].x, mesh.colors.size() * sizeof(glm::vec4), Buffer::FillType::Once);
		mBuffers.push_back(buffer);
	}
}

void Model::SortVertexData(Mesh& newMesh, const Mesh& oldMesh, const std::vector<Face>& faces)
{
	GLuint count = 0;
	std::unordered_map<VertexGroup, GLuint, HashFunction> map;

	for (const auto& face : faces)
	{
		for (auto i = 0; i < 3; i++)
		{
			auto it = map.find(face[i]);

			// new face
			if (it == map.end())
			{
				newMesh.vertices.push_back(oldMesh.vertices[face[i].v]);
				newMesh.texCoords.push_back(oldMesh.texCoords[face[i].t]);
				newMesh.normals.push_back(oldMesh.normals[face[i].n]);
				newMesh.indices.push_back(count);

				map[face[i]] = count;
				count++;
			}
			else
			{
				newMesh.indices.push_back(it->second);
			}
		}
	}
}

void Model::Update(float deltaTime)
{

}

void Model::Render(const Shader& shader)
{
	Object::Render(shader);

	shader.SetFloat("material.shininess", mShininess);
	shader.SetVec3("material.ambient", mAmbient);
	shader.SetVec3("material.diffuse", mDiffuse);
	shader.SetVec3("material.specular", mSpecular);
	shader.SetBool("bTextured", false);

	int count = 0;
	for (auto& buffer : mBuffers)
	{
		buffer.LinkEBO();
		buffer.LinkVBO(shader, "aPos", Buffer::VBOType::VertexBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader, "aColor", Buffer::VBOType::ColorBuffer, Buffer::ComponentType::RGBA, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader, "aTexCoords", Buffer::VBOType::TextureBuffer, Buffer::ComponentType::UV, Buffer::DataType::FloatData);
		buffer.LinkVBO(shader, "aNormal", Buffer::VBOType::NormalBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::FloatData);
		for (auto mat : mMaterials)
		{
			if (mat.GetName() == mMeshes[count].materialName)
			{
				mat.SendToShader(shader);
				if (mat.IsTextured())
				{
					shader.SetBool("bTextured", true);
					mat.GetDiffuseMap().Bind();
				}
				else
				{
					shader.SetBool("bTextured", false);
				}
			}
		}
		count++;
		buffer.Draw(Buffer::DrawType::Triangles);
	}
}

