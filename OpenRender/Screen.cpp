#include "Screen.h"
#include <glad/glad.h>
#include <cstdio>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"

Screen* Screen::Instance()
{
	static Screen* screen = new Screen;
	return screen;
}

Screen::Screen()
{
	mWindow = nullptr;
	mContext = nullptr;
}

bool Screen::Init(int width, int height)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("ERROR: Failed to initialize SDL: %s\n", SDL_GetError());
		return false;
	}

	// Set OpenGL Settings
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Set Core 400 profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	mWidth = width;
	mHeight = height;
	mWindow = SDL_CreateWindow("OpenRender",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		mWidth, mHeight,
		SDL_WINDOW_OPENGL);
	if (!mWindow)
	{
		printf("ERROR: Failed to create SDL Window: %s\n", SDL_GetError());
		return false;
	}

	mContext = SDL_GL_CreateContext(mWindow);
	if (!mContext)
	{
		printf("ERROR: Failed to create OpenGL Context: %s\n", SDL_GetError());
		return false;
	}
	SDL_GL_MakeCurrent(mWindow, mContext);

	// Initilaize glad
	if (!gladLoadGL())
	{
		printf("ERROR: Failed to load extensions.\n");
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init("#version 460");
	ImGui_ImplSDL2_InitForOpenGL(mWindow, mContext);

    return true;
}

void Screen::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::Present()
{
	SDL_GL_SwapWindow(mWindow);
}

void Screen::Shutdown()
{
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
