#pragma once
#include <glad/glad.h>
#include <string>
class Texture
{
public:
	Texture();
	void Bind(unsigned int slot = 0) const;
	bool Load(const std::string& path);
	void Unbind();
	void Unload();
private:
	unsigned int mID;
};

