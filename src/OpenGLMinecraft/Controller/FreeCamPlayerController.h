#pragma once

#include "OpenGLMinecraft/Controller/PlayerController.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"


// basic freecam player controller, base class for CreativePlayer and SurvivalPlayer
class FreeCamPlayerController : public PlayerController
{
public:
    FreeCamPlayerController(const RenderWindow::WindowData& p_WindowData, glm::vec3 p_Position = {0.0f, 0.0f, 0.0f}, float p_Yaw = -90.0f, float p_Pitch = 0.0f);

    virtual void HandleInput(const Mouse* p_Mouse, const Keyboard* p_Keyboard, double p_DeltaTime) override;

protected:
    inline void Rotate(float p_YawOffset, float p_PitchOffset) { m_Yaw += p_YawOffset; m_Pitch += p_PitchOffset; }
    inline void MoveForward(float p_Amount) { m_Position += m_Front * p_Amount; }
    inline void MoveBack(float p_Amount) { m_Position -= m_Front * p_Amount; }
    inline void MoveLeft(float p_Amount) { m_Position -= m_Right * p_Amount; }
    inline void MoveRight(float p_Amount) { m_Position += m_Right * p_Amount; }
    void UpdateCamera();

    static constexpr glm::vec3 s_WorldUp = {0.0f, 1.0f, 0.0f};

    float m_Yaw;
    float m_Pitch;

    glm::vec3 m_Front;
    glm::vec3 m_Right;
    glm::vec3 m_Up;
};

