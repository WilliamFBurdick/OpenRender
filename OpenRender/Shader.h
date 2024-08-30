#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
	void SetVec3(const std::string& name, float v1, float v2, float v3) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetMat4(const std::string& name, glm::mat4 matrix) const;
};

