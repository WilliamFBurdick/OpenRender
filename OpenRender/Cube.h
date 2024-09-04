#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Material.h"
#include "Object.h"

class Cube : public Object
{
public:
	Cube(const std::string& texturePath = "");
	~Cube();

	void Update(float deltaTime) override;
	void Render(const Shader& shader) override;
private:
	Buffer mBuffer;
	Texture mTexture;
	Material mMaterial;
};

