#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Camera.h"
#include "OpenGLMinecraft/MultiMedia/Input/Keyboard.h"
#include "OpenGLMinecraft/MultiMedia/Input/Mouse.h"

class Player
{
public:
    Player(const RenderWindow::WindowData& p_WindowData, glm::vec3 p_Position = {0.0f, 0.0f, 0.0f}, float p_Yaw = -90.0f, float p_Pitch = 0.0f);

    void HandleInput(const Mouse* p_Mouse, const Keyboard* p_Keyboard, double p_DeltaTime);

    inline const Camera* GetCamera() { return m_PlayerCam.get(); };

private:
    inline void Rotate(float p_YawOffset, float p_PitchOffset) { m_Yaw += p_YawOffset; m_Pitch += p_PitchOffset; }
    inline void MoveForward(float p_Amount) { m_Position += m_Front * p_Amount; }
    inline void MoveBack(float p_Amount) { m_Position -= m_Front * p_Amount; }
    inline void MoveLeft(float p_Amount) { m_Position -= m_Right * p_Amount; }
    inline void MoveRight(float p_Amount) { m_Position += m_Right * p_Amount; }
    void UpdateCamera();

    static constexpr glm::vec3 s_WorldUp = {0.0f, 1.0f, 0.0f};

    glm::vec3 m_Position;
    float m_Yaw;
    float m_Pitch;

    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;

    std::unique_ptr<Camera> m_PlayerCam;
};

