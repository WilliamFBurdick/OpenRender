#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Material
{
public:
	Material();
	inline const std::string& GetName() const { return mName; }
	inline bool IsTextured() const { return mTextured; }

	inline const Texture& GetNormalMap() const { return mNormalMap; }
	inline const Texture& GetAmbientMap() const { return mAmbientMap; }
	inline const Texture& GetDiffuseMap() const { return mDiffuseMap; }
	inline const Texture& GetSpecularMap() const { return mSpecularMap; }

	void SetShininess(GLfloat shininess);
	void SetAmbient(const glm::vec3& ambient);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetSpecular(const glm::vec3& specular);

	bool Load(const std::string& path, std::vector<Material>& materials);

	void SendToShader(const Shader& shader) const;
private:
	bool mTextured;
	std::string mName;

	Texture mNormalMap, mAmbientMap, mDiffuseMap, mSpecularMap;

	GLfloat mShininess;
	glm::vec3 mAmbient, mDiffuse, mSpecular;
};

