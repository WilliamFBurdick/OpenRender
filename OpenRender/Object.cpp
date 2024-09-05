#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>

Object::Object(Transform* parent)
{
	mParent = parent;
	mTransform = Transform();
}

void Object::Render(const Shader& shader)
{
	glm::mat4 model;
	if (mParent)
	{
		model = mParent->GetModel() * mTransform.GetModel();
	}
	else
	{
		model = mTransform.GetModel();
	}

	glm::mat3 normal = glm::inverse(glm::mat3(model));

	shader.SetMat4("model", model);
	shader.SetMat3("normal", normal);
}

void Object::SetParent(Transform* parent)
{
	mParent = parent;
}
