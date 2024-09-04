#include "Buffer.h"
#include "Shader.h"

Buffer::Buffer():
	mVAO(0), mVertexPosVBO(0), mVertexColorVBO(0), mVertexCount(0), mTexCoordsVBO(0), mEBO(0), mHasEBO(false)
{
}

void Buffer::CreateBuffer(unsigned int numVertices, bool bHasEBO)
{
	mHasEBO = bHasEBO;
	glGenBuffers(1, &mVertexPosVBO);
	glGenBuffers(1, &mVertexColorVBO);
	glGenBuffers(1, &mTexCoordsVBO);
	glGenBuffers(1, &mVertexNormalVBO);
	glGenVertexArrays(1, &mVAO);
	if (mHasEBO)
	{
		glGenBuffers(1, &mEBO);
	}
	mVertexCount = numVertices;
}

void Buffer::FillVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, FillType fillType)
{
	glBindVertexArray(mVAO);
	if (vboType == VBOType::VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexPosVBO);
	}
	else if (vboType == VBOType::ColorBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexColorVBO);
	}
	else if (vboType == VBOType::TextureBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsVBO);
	}
	else if (vboType == VBOType::NormalBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalVBO);
	}
	glBufferData(GL_ARRAY_BUFFER, bufferSize, data, (GLint)fillType);
	glBindVertexArray(0);
}

void Buffer::FillEBO(GLuint* data, GLsizeiptr bufferSize, FillType fillType)
{
	if (!mHasEBO) return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, data, (GLenum)fillType);
}

void Buffer::AppendVBO(VBOType vboType, const void* data, GLsizeiptr bufferSize, GLuint offset)
{
	glBindVertexArray(mVAO);
	if (vboType == VBOType::VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexPosVBO);
	}
	else if (vboType == VBOType::ColorBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexColorVBO);
	}
	else if (vboType == VBOType::TextureBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsVBO);
	}
	else if (vboType == VBOType::NormalBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalVBO);
	}
	glBufferSubData(GL_ARRAY_BUFFER, offset, bufferSize, data);
	glBindVertexArray(0);

}

void Buffer::LinkVBO(const Shader& shader, const std::string& attribute, VBOType vboType, ComponentType componentType, DataType dataType)
{
	glBindVertexArray(mVAO);
	unsigned int attributeID = glGetAttribLocation(shader.ID, attribute.c_str());
	if (vboType == VBOType::VertexBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexPosVBO);
	}
	else if (vboType == VBOType::ColorBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexColorVBO);
	}
	else if (vboType == VBOType::TextureBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mTexCoordsVBO);
	}
	else if (vboType == VBOType::NormalBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexNormalVBO);
	}
	glVertexAttribPointer(attributeID, (GLint)componentType, (GLenum)dataType, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(attributeID);
	glBindVertexArray(0);
}

void Buffer::LinkEBO()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBindVertexArray(0);
}

void Buffer::Draw(DrawType drawType)
{
	glBindVertexArray(mVAO);
	if (mHasEBO)
	{
		glDrawElements((GLenum)drawType, mVertexCount, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays((GLint)drawType, 0, mVertexCount);
	}
	glBindVertexArray(0);
}

void Buffer::DestroyBuffer()
{
	glDeleteBuffers(1, &mVertexPosVBO);
	glDeleteBuffers(1, &mVertexColorVBO);
	glDeleteBuffers(1, &mTexCoordsVBO);
	glDeleteBuffers(1, &mVertexNormalVBO);
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mEBO);
}
