#pragma once
#include <glm/glm.hpp>
#include "Buffer.h"
#include "Shader.h"
#include "Transform.h"
class Grid
{
public:
	Grid();

	inline Transform& GetTransform() { return mTransform; }

	void Update(float deltaTime);
	void Render(const Shader& shader);
private:
	Buffer mBuffer;
	Transform mTransform;
};

