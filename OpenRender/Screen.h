#pragma once
#include <SDL2/SDL.h>
class Screen
{
public:
	static Screen* Instance();
public:
	bool Init();
	void Clear();
	void Present();
	void Shutdown();
private:
	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);
private:
	SDL_Window* mWindow;
	SDL_GLContext mContext;
};

