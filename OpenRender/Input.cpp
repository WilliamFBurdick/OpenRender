#include "Input.h"

Input* Input::Instance()
{
    static Input* input = new Input();
    return input;
}

Input::Input()
{
    m_isXClicked = false;
}

void Input::Update()
{
    // Reset mouse input variables (prevent sticking)
    m_mouseMotionX = 0;
    m_mouseMotionY = 0;
    m_mousePositionX = 0;
    m_mousePositionY = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                m_isXClicked = true;
                break;
            case SDL_KEYDOWN:
                m_isKeyPressed = true;
                m_keyDown = event.key.keysym.sym;
                break;
            case SDL_KEYUP:
                m_isKeyPressed = false;
                m_keyUp = event.key.keysym.sym;
                break;
            case SDL_MOUSEBUTTONDOWN:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        m_isLeftButtonClicked = true;
                        break;
                    case SDL_BUTTON_RIGHT:
                        m_isRightButtonClicked = true;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        m_isMiddleButtonClicked = true;
                        break;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                switch (event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        m_isLeftButtonClicked = false;
                        break;
                    case SDL_BUTTON_RIGHT:
                        m_isRightButtonClicked = false;
                        break;
                    case SDL_BUTTON_MIDDLE:
                        m_isMiddleButtonClicked = false;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                m_mouseMotionX = event.motion.xrel;
                m_mouseMotionY = event.motion.yrel;

                m_mousePositionX = event.motion.x;
                m_mousePositionY = event.motion.y;
                break;
        }
    }
    mKeyboard = SDL_GetKeyboardState(NULL);
}
