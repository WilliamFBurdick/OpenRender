#pragma once
#include <SDL2/SDL.h>
class Input
{
public:
	static Input* Instance();
public:
	inline bool IsXClicked() { return m_isXClicked; }
	inline bool IsKeyPressed() { return m_isKeyPressed; }

	inline char GetKeyUp() { return m_keyUp; }
	inline char GetKeyDown() { return m_keyDown; }

	inline bool IsLeftButtonClicked() { return m_isLeftButtonClicked; }
	inline bool IsRightButtonClicked() { return m_isRightButtonClicked; }
	inline bool IsMiddleMouseButtonClicked() { return m_isMiddleButtonClicked; }

	inline int GetMousePositionX() { return m_mousePositionX; }
	inline int GetMousePositionY() { return m_mousePositionY; }

	inline int GetMouseMotionX() { return m_mouseMotionX; }
	inline int GetMouseMotionY() { return m_mouseMotionY; }
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
};

