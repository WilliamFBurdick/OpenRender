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

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int CONSOLE_WINDOW_HEIGHT = 250;
const int PROPERTIES_WINDOW_WIDTH = 400;

float deltaTime, lastFrame;

SDL_Rect mouseCollider = { 0 };
SDL_Rect sceneCollider = { 0, 0, SCREEN_WIDTH - PROPERTIES_WINDOW_WIDTH, SCREEN_HEIGHT - CONSOLE_WINDOW_HEIGHT };

void RenderConsoleWindow()
{
	ImGui::Begin("Output console", nullptr, 
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImVec2 windowPos = ImVec2(0, SCREEN_HEIGHT - CONSOLE_WINDOW_HEIGHT - 25);
	ImVec2 windowSize = ImVec2(SCREEN_WIDTH - PROPERTIES_WINDOW_WIDTH, CONSOLE_WINDOW_HEIGHT);

	ImGui::SetWindowPos("Output console", windowPos);
	ImGui::SetWindowSize("Output console", windowSize);

	ImGui::End();
}

void RenderPropertiesWindow()
{
	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	ImVec2 windowPos = ImVec2(SCREEN_WIDTH - PROPERTIES_WINDOW_WIDTH, 0);
	ImVec2 windowSize = ImVec2(PROPERTIES_WINDOW_WIDTH, SCREEN_HEIGHT);

	ImGui::SetWindowPos("Properties", windowPos);
	ImGui::SetWindowSize("Properties", windowSize);

	ImGui::End();
}

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
	camera.SetViewport(0, CONSOLE_WINDOW_HEIGHT, SCREEN_WIDTH - PROPERTIES_WINDOW_WIDTH, SCREEN_HEIGHT - CONSOLE_WINDOW_HEIGHT);

	//=========================================================================
	// MAIN LOOP
	//=========================================================================
	bool isRunning = true;
	while (isRunning)
	{
		float currentFrame = SDL_GetTicks() / 1000.f;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Input::Instance()->Update();

		mouseCollider = { Input::Instance()->GetMousePositionX(), Input::Instance()->GetMousePositionY(), 1, 1 };
		bool bMouseColliding = SDL_HasIntersection(&mouseCollider, &sceneCollider);

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)Screen::Instance()->GetWidth() / (float)Screen::Instance()->GetHeight(), 0.1f, 100.f);

		if (Input::Instance()->IsXClicked() || Input::Instance()->GetKeyboard()[SDL_SCANCODE_ESCAPE])
		{
			isRunning = false;
		}
		Screen::Instance()->Clear();

		if (bMouseColliding && Input::Instance()->IsLeftButtonClicked())
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

		RenderConsoleWindow();
		RenderPropertiesWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		Screen::Instance()->Present();
	}
	Screen::Instance()->Shutdown();
	return 0;
}