#include "Grid.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Input.h"

Grid::Grid()
{
	const auto SIZE = 10;
	const auto QUADRANTS = 4;
	const auto VERTICES_PER_LINE = 2;

	const auto BYTES_PER_LINE_VERTEX = VERTICES_PER_LINE * (unsigned int)(Buffer::ComponentType::XYZ) * sizeof(GLint);
	const auto BYTES_PER_LINE_COLOR = VERTICES_PER_LINE * (unsigned int)(Buffer::ComponentType::RGBA) * sizeof(GLfloat);

	const auto BYTES_VERTEX_VBO = SIZE * QUADRANTS * BYTES_PER_LINE_VERTEX;
	const auto BYTES_COLOR_VBO = SIZE * QUADRANTS * BYTES_PER_LINE_COLOR;

	mBuffer.CreateBuffer(SIZE * QUADRANTS * VERTICES_PER_LINE, false);
	mBuffer.FillVBO(Buffer::VBOType::VertexBuffer, (GLint*)nullptr, BYTES_VERTEX_VBO, Buffer::FillType::Ongoing);
	mBuffer.FillVBO(Buffer::VBOType::ColorBuffer, (GLfloat*)nullptr, BYTES_COLOR_VBO, Buffer::FillType::Ongoing);

	GLuint offsetColor = 0;
	GLuint offsetVertex = 0;

	// =======================
	// Negative vertical lines
	// =======================
	for (int i = 0; i <= SIZE; i++)
	{
		GLint vertices[] = { -SIZE + i, 0, SIZE,
							-SIZE + i , 0, -SIZE };
		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 1.0f,
							0.5f, 0.5f, 0.5f, 1.0f };
		mBuffer.AppendVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		mBuffer.AppendVBO(Buffer::VBOType::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += BYTES_PER_LINE_VERTEX;
		offsetColor += BYTES_PER_LINE_COLOR;
	}
	// =======================
	// Positive vertical lines
	// =======================
	for (int i = 1; i < SIZE; i++)
	{
		GLint vertices[] = { 0 + i, 0, SIZE,
							0 + i , 0, -SIZE };
		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 1.0f,
							0.5f, 0.5f, 0.5f, 1.0f };
		mBuffer.AppendVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		mBuffer.AppendVBO(Buffer::VBOType::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += BYTES_PER_LINE_VERTEX;
		offsetColor += BYTES_PER_LINE_COLOR;
	}
	// =======================
	// Negative horizontal lines
	// =======================
	for (int i = 0; i <= SIZE; i++)
	{
		GLint vertices[] = { -SIZE, 0, -SIZE + i,
							SIZE , 0, -SIZE + i };
		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 1.0f,
							0.5f, 0.5f, 0.5f, 1.0f };
		mBuffer.AppendVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		mBuffer.AppendVBO(Buffer::VBOType::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += BYTES_PER_LINE_VERTEX;
		offsetColor += BYTES_PER_LINE_COLOR;
	}
	// =======================
	// Positive horizontal lines
	// =======================
	for (int i = 1; i < SIZE; i++)
	{
		GLint vertices[] = { -SIZE, 0, 0 + i,
							SIZE , 0, 0 + i };
		GLfloat colors[] = { 0.5f, 0.5f, 0.5f, 1.0f,
							0.5f, 0.5f, 0.5f, 1.0f };
		mBuffer.AppendVBO(Buffer::VBOType::VertexBuffer, vertices, sizeof(vertices), offsetVertex);
		mBuffer.AppendVBO(Buffer::VBOType::ColorBuffer, colors, sizeof(colors), offsetColor);

		offsetVertex += BYTES_PER_LINE_VERTEX;
		offsetColor += BYTES_PER_LINE_COLOR;
	}
	//
}

void Grid::Update(float deltaTime)
{

}

void Grid::Render(const Shader& shader)
{
	glm::mat4 model = mTransform.GetModel();

	shader.SetMat4("model", model);
	shader.SetVec3("color", glm::vec3(0.5f, 0.5f, 0.0f));

	mBuffer.LinkEBO();
	mBuffer.LinkVBO(shader, "aPos", Buffer::VBOType::VertexBuffer, Buffer::ComponentType::XYZ, Buffer::DataType::IntData);
	//mBuffer.LinkVBO(shader, "aColor", Buffer::VBOType::ColorBuffer, Buffer::ComponentType::RGBA, Buffer::DataType::FloatData);

	mBuffer.Draw(Buffer::DrawType::Lines);
}
