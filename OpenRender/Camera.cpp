#include "Camera.h"
#include "Input.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVITY),
    Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix(float screenWidth, float screenHeight) const
{
    glm::mat4 projection = glm::perspective(Zoom, screenWidth / screenHeight, 0.1f, 100.0f);
    return projection;
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    switch (direction)
    {
    case EForward:
        Position += Front * velocity;
        break;
    case EBackward:
        Position -= Front * velocity;
        break;
    case ERight:
        Position += Right * velocity;
        break;
    case ELeft:
        Position -= Right * velocity;
        break;
    case EUp:
        Position += Up * velocity;
        break;
    case EDown:
        Position -= Up * velocity;
        break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch -= yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::SetViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    glViewport(x, y, width, height);
}

void Camera::Update(float deltaTime)
{
    //ProcessMouseMovement(Input::Instance()->GetMouseMotionX(), Input::Instance()->GetMouseMotionY());
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_W])
    {
        ProcessKeyboard(Camera_Movement::EForward, deltaTime);
    }
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_A])
    {
        ProcessKeyboard(Camera_Movement::ELeft, deltaTime);
    }
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_S])
    {
        ProcessKeyboard(Camera_Movement::EBackward, deltaTime);
    }
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_D])
    {
        ProcessKeyboard(Camera_Movement::ERight, deltaTime);
    }
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_SPACE])
    {
        ProcessKeyboard(Camera_Movement::EUp, deltaTime);
    }
    if (Input::Instance()->GetKeyboard()[SDL_SCANCODE_LCTRL])
    {
        ProcessKeyboard(Camera_Movement::EDown, deltaTime);
    }
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
