#include "Light.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>

Light::Light(const Shader& shader)
{
	mAmbient = glm::vec3(1.0f);
	mDiffuse = glm::vec3(1.0f);
	mSpecular = glm::vec3(1.0f);

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};
	
	mBuffer.CreateBuffer(36);

	mBuffer.FillVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), Buffer::FillType::Once);
	mBuffer.LinkVBO(shader, "aPos", Buffer::VBOType::VertexBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::FloatData);

	shader.SetVec3("color", glm::vec3(1.f, 1.f, 1.f));
}

Light::~Light()
{
	mBuffer.DestroyBuffer();
}

void Light::Update(float deltaTime)
{

}

void Light::Render(Shader& shader)
{
	glm::mat4 model = mTransform.GetModel();
	shader.SetMat4("model", model);
	shader.SetVec3("color", glm::vec3(1.f, 1.f, 1.f));
	shader.use();
	mBuffer.Draw(Buffer::DrawType::Triangles);
}

void Light::SendToShader(const Shader& shader)
{
	shader.SetVec3("light.position", mTransform.GetPosition());
	shader.SetVec3("light.ambient", mAmbient);
	shader.SetVec3("light.diffuse", mDiffuse);
	shader.SetVec3("light.specular", mSpecular);
}
