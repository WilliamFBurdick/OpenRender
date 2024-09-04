#pragma once

#include <glm/glm.hpp>
#include "Buffer.h"
#include "Shader.h"
#include "Transform.h"

class Object
{
public:
	Object(Transform* parent = nullptr);
	virtual ~Object() = 0 {}

	Transform& GetTransform() { return mTransform; }
	void SetParent(Transform* parent);

	virtual void Update(float deltaTime) = 0;
	virtual void Render(const Shader& shader);
private:
	Transform* mParent;
	Buffer mBuffer;
	Transform mTransform;
};

