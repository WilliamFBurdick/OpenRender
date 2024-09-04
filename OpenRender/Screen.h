#pragma once
#include <SDL2/SDL.h>
class Screen
{
public:
	static Screen* Instance();
public:
	bool Init(int width = 1920, int height = 1080);
	void Clear();
	void Present();
	void Shutdown();

	inline const int GetWidth() { return mWidth; }
	inline const int GetHeight() { return mHeight; }
private:
	Screen();
	Screen(const Screen&);
	Screen& operator=(const Screen&);

	int mWidth, mHeight;
private:
	SDL_Window* mWindow;
	SDL_GLContext mContext;
};

