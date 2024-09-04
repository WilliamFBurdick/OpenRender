#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale):
	mPosition(position), mRotation(rotation), mScale(scale)
{
	mDirty = true;
	mMatrix = glm::mat4(1.0f);
}

const glm::mat4& Transform::GetModel()
{
	if (mDirty)
	{
		// rebuild model matrix
		mMatrix = glm::mat4(1.0f);
		mMatrix = glm::translate(mMatrix, mPosition);
		mMatrix = glm::rotate(mMatrix, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		mMatrix = glm::rotate(mMatrix, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		mMatrix = glm::rotate(mMatrix, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		mMatrix = glm::scale(mMatrix, mScale);
		mDirty = false;
	}
	return mMatrix;
}

void Transform::SetIdentity()
{
	mPosition = glm::vec3(0.0f);
	mRotation = glm::vec3(0.0f);
	mScale = glm::vec3(1.0f);
	mDirty = true;
}

void Transform::SetPosition(glm::vec3 position)
{
	mPosition = position;
	mDirty = true;
}

void Transform::SetRotation(glm::vec3 rotation)
{
	mRotation = rotation;
	mDirty = true;
}

void Transform::SetScale(glm::vec3 scale)
{
	mScale = scale;
	mDirty = true;
}
