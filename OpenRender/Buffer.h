#pragma once
#include <glad/glad.h>
#include <string>
class Buffer
{
public:
	enum class VBOType 
	{
		VertexBuffer,
		ColorBuffer,
		TextureBuffer,
		NormalBuffer
	};

	enum class ComponentType
	{
		XY = 2,
		XYZ = 3,
		RGB = 3,
		RGBA = 4,
		UV = 2
	};

	enum class FillType
	{
		Once = GL_STATIC_DRAW,
		Ongoing = GL_DYNAMIC_DRAW
	};

	enum class DataType
	{
		IntData = GL_INT,
		FloatData = GL_FLOAT,
		UnsignedIntData = GL_UNSIGNED_INT
	};

	enum class DrawType
	{
		Points = GL_POINTS,
		Triangles = GL_TRIANGLES,
		Lines = GL_LINES
	};
public:
	Buffer();
public:
	void CreateBuffer(unsigned int numVertices, bool bHasEBO = false);
	void FillVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, FillType fillType);
	void FillEBO(GLuint* data, GLsizeiptr bufferSize, FillType fillType = FillType::Once);
	void AppendVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, GLuint offset);
	void LinkVBO(const class Shader& shader, const std::string& attribute, VBOType vboType, ComponentType componentType, DataType dataType);
	void LinkEBO();
	void Draw(DrawType drawType);
	void DestroyBuffer();
private:
	bool mHasEBO;
	unsigned int mVAO, mEBO;
	unsigned int mVertexPosVBO, mVertexColorVBO, mTexCoordsVBO, mVertexNormalVBO;
	unsigned int mVertexCount;
};

