#pragma once
#include "Buffer.h"
#include "Texture.h"
#include "Object.h"
#include "Material.h"

class Quad : public Object
{
public:
	Quad();
	~Quad();

	void Render(const Shader& shader) override;
	void Update(float deltaTime) override;
private:
	Buffer mBuffer;
	Texture mTexture;
	Material mMaterial;
};

