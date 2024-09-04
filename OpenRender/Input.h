#pragma once
#include <SDL2/SDL.h>
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl2.h"
class Input
{
public:
	static Input* Instance();
public:
	inline bool IsXClicked() const { return m_isXClicked; }
	inline bool IsKeyPressed() const { return m_isKeyPressed; }

	inline char GetKeyUp() const { return m_keyUp; }
	inline char GetKeyDown() const { return m_keyDown; }

	inline bool IsLeftButtonClicked() const { return m_isLeftButtonClicked; }
	inline bool IsRightButtonClicked() const { return m_isRightButtonClicked; }
	inline bool IsMiddleMouseButtonClicked() const { return m_isMiddleButtonClicked; }

	inline int GetMousePositionX() { return m_mousePositionX; }
	inline int GetMousePositionY() { return m_mousePositionY; }

	inline int GetMouseMotionX() { return m_mouseMotionX; }
	inline int GetMouseMotionY() { return m_mouseMotionY; }

	inline const Uint8* GetKeyboard() { return mKeyboard; }
public:
	void Update();
private:
	Input();
	Input(const Input&);
	Input& operator=(Input&);
private:
	char m_keyUp;
	char m_keyDown;

	bool m_isXClicked;
	bool m_isKeyPressed;

	bool m_isLeftButtonClicked;
	bool m_isRightButtonClicked;
	bool m_isMiddleButtonClicked;

	int m_mouseMotionX;
	int m_mouseMotionY;

	int m_mousePositionX;
	int m_mousePositionY;

	const Uint8* mKeyboard;
};

