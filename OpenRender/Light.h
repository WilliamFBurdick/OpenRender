#pragma once

#include <glm/glm.hpp>
#include "Buffer.h"
#include "Transform.h"

class Light
{
public:
	Light(const class Shader& shader);
	~Light();

	void Update(float deltaTime);
	void Render(Shader& shader);
	void SendToShader(const Shader& shader);
private:
	Buffer mBuffer;
	Transform mTransform;
	
	glm::vec3 mAmbient, mDiffuse, mSpecular;
};

