#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Transform
{
public:
	Transform(const glm::vec3& position = glm::vec3(0.f), const glm::vec3& rotation = glm::vec3(0.f), const glm::vec3& scale = glm::vec3(1.f));

	inline const glm::vec3& GetPosition() const { return mPosition; }
	inline const glm::vec3& GetRotation() const { return mRotation; }
	inline const glm::vec3& GetScale() const { return mScale; }
	const glm::mat4& GetModel();

	void SetIdentity();
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	void SetScale(glm::vec3 scale);
private:
	bool mDirty;
	glm::vec3 mPosition, mRotation, mScale;
	glm::mat4 mMatrix;
};

