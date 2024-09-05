#include "Quad.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

Quad::Quad():
	Object()
{
	GLfloat vertices[] = { -0.5f,  0.5f, 0.0f,
							0.5f,  0.5f, 0.0f,
							0.5f, -0.5f, 0.0f,
						   -0.5f, -0.5f, 0.0f };

	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f,
						 0.0f, 0.0f, 1.0f, 1.0f,
						 0.0f, 1.0f, 0.0f, 1.0f,
						 0.0f, 1.0f, 1.0f, 1.0f };

	GLfloat UVs[] = { 0.0f, 1.0f,
					  1.0f, 1.0f,
					  1.0f, 0.0f,
					  0.0f, 0.0f };

	// index buffer
	GLuint indices[] = { 0, 1, 3,
						 3, 1, 2 };

	GLfloat normals[] = {
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f,
					0.0f, 0.0f, 1.0f
	};

	mBuffer.CreateBuffer(6, true);
	mBuffer.FillEBO(indices, sizeof(indices), Buffer::FillType::Once);
	mBuffer.FillVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), Buffer::FillType::Once);
	mBuffer.FillVBO(Buffer::VBOType::ColorBuffer, colors, sizeof(colors), Buffer::FillType::Once);
	mBuffer.FillVBO(Buffer::VBOType::TextureBuffer, UVs, sizeof(UVs), Buffer::FillType::Once);
	mBuffer.FillVBO(Buffer::VBOType::NormalBuffer, normals, sizeof(normals), Buffer::FillType::Once);

	mTexture.Load("resources/textures/Crate_1.png");

	mMaterial.SetAmbient(glm::vec3(0.4f, 0.4f, 0.4f));
	mMaterial.SetDiffuse(glm::vec3(0.1f, 0.7f, 0.2f));
	mMaterial.SetSpecular(glm::vec3(0.8f, 0.8f, 0.8f));
	mMaterial.SetShininess(50.f);
}

Quad::~Quad()
{
	mBuffer.DestroyBuffer();
}

void Quad::Render(const Shader& shader)
{
	Object::Render(shader);
	mMaterial.SendToShader(shader);

	mBuffer.LinkEBO();
	mBuffer.LinkVBO(shader, "aPos", Buffer::VBOType::VertexBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::FloatData);
	mBuffer.LinkVBO(shader, "aColor", Buffer::VBOType::ColorBuffer, Buffer::ComponentType::RGBA, Buffer::DataType::FloatData);
	mBuffer.LinkVBO(shader, "aTexCoords", Buffer::VBOType::TextureBuffer, Buffer::ComponentType::UV, Buffer::DataType::FloatData);
	mBuffer.LinkVBO(shader, "aNormal", Buffer::VBOType::NormalBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::FloatData);

	mTexture.Bind(0);
	mBuffer.Draw(Buffer::DrawType::Triangles);
	mTexture.Unbind();
}

void Quad::Update(float deltaTime)
{
}
