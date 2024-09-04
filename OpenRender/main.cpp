#include <cstdio>
#include <glad/glad.h>
#include "Screen.h"
#include "Input.h"
#include "Shader.h"
#include "Quad.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "Model.h"
#include "Grid.h"

#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"

float deltaTime, lastFrame;

void ProcessInput(float deltaTime);

int main(int argc, char* argv[])
{
	if (!Screen::Instance()->Init())
	{
		return -1;
	}

	//=========================================================================
	// INITIALIZING
	//=========================================================================

	Shader litShader("shaders/lit.vert", "shaders/lit.frag");
	Shader unlitShader("shaders/unlit.vert", "shaders/unlit.frag");

	Grid grid;
	std::vector<Object*> objects;

	Quad quad;
	objects.push_back(&quad);

	Cube cube("resources/textures/Crate_1.png");
	cube.GetTransform().SetPosition(glm::vec3(-1.0f, 0.0f, -2.0f));
	cube.GetTransform().SetRotation(glm::vec3(0.0f, 45.0f, 0.0f));
	cube.GetTransform().SetScale(glm::vec3(0.5f));
	cube.SetParent(&grid.GetTransform());
	objects.push_back(&cube);
	Model model;
	model.Load("resources/objects/Armchair.obj");
	objects.push_back(&model);


	Light light(unlitShader);
	Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));

	//=========================================================================
	// MAIN LOOP
	//=========================================================================
	bool isRunning = true;
	while (isRunning)
	{
		float currentFrame = SDL_GetTicks() / 1000.f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)Screen::Instance()->GetWidth() / (float)Screen::Instance()->GetHeight(), 0.1f, 100.f);

		Input::Instance()->Update();
		if (Input::Instance()->IsXClicked() || Input::Instance()->GetKeyboard()[SDL_SCANCODE_ESCAPE])
		{
			isRunning = false;
		}
		Screen::Instance()->Clear();

		if (Input::Instance()->IsLeftButtonClicked())
		{
			glm::vec3 rotation = grid.GetTransform().GetRotation();
			rotation.x += Input::Instance()->GetMouseMotionY() * deltaTime;
			rotation.y += Input::Instance()->GetMouseMotionX() * deltaTime;
			grid.GetTransform().SetRotation(rotation);
		}

		camera.Update(deltaTime);
		light.Update(deltaTime);
		cube.Update(deltaTime);

		// Render the light
		unlitShader.use();
		unlitShader.SetMat4("view", camera.GetViewMatrix());
		unlitShader.SetMat4("projection", projection);
		light.Render(unlitShader);

		grid.Update(deltaTime);
		grid.Render(unlitShader);

		// Render the quad
		litShader.use();
		light.SendToShader(litShader);
		litShader.SetMat4("view", camera.GetViewMatrix());
		litShader.SetMat4("projection", projection);
		litShader.SetVec3("viewPos", camera.Position);

		for (auto& object : objects)
		{
			object->Render(litShader);
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Screen::Instance()->Present();
	}
	Screen::Instance()->Shutdown();
	return 0;
}