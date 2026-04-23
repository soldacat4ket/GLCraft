#include "Player.h"

#include <algorithm>

#include "OpenGLMinecraft/Config.h"

Player::Player(const RenderWindow::WindowData& p_WindowData, const glm::vec3 p_Position, const float p_Yaw, const float p_Pitch)
    :m_Position(p_Position), m_Yaw(p_Yaw), m_Pitch(p_Pitch)
{
    const float CameraNearPlane = 0.1f;
    m_PlayerCam = std::make_unique<Camera>(p_WindowData.AspectRatioF, // aspect ratio
                                           CameraNearPlane, // near plane
                                           Config::Get().GetGraphicsSettings().RenderDistanceBlocks, // far plane
                                           Config::Get().GetGraphicsSettings().DesiredFov); // fov
    UpdateCamera();
}

void Player::HandleInput(const Mouse* p_Mouse, const Keyboard* p_Keyboard, const double p_DeltaTime)
{

    const Mouse::MousePos& MouseDelta = p_Mouse->GetState().MouseDelta;

    const float FlatLookAmount = 0.1f;
    float FinalLookDirection = FlatLookAmount * Config::Get().GetInputSettings().UserSensitivity;
    float YawOffset = static_cast<float>(MouseDelta.x) * FinalLookDirection;
    float PitchOffset = static_cast<float>(MouseDelta.y) * FinalLookDirection;

    Rotate(YawOffset, PitchOffset);

    // clamp the pitch since weird things happen if the front and up vector align
    m_Pitch = std::clamp(m_Pitch, -89.0f, 90.0f);

    const float MovementSpeed = 10.0f;
    float ScaledCamMovement = MovementSpeed * (float)p_DeltaTime;
    if(p_Keyboard->IsKeyPressed(GLFW_KEY_W)) MoveForward(ScaledCamMovement);
    if(p_Keyboard->IsKeyPressed(GLFW_KEY_S)) MoveBack(ScaledCamMovement);
    if(p_Keyboard->IsKeyPressed(GLFW_KEY_A)) MoveLeft(ScaledCamMovement);
    if(p_Keyboard->IsKeyPressed(GLFW_KEY_D)) MoveRight(ScaledCamMovement);

    UpdateCamera();

}

void Player::UpdateCamera()
{
    m_PlayerCam->SetPosition(m_Position + glm::vec3(0.0f, 1.0f, 0.0f)); // set camera at 1 block higher than the players position
    
    const glm::vec3 NewFront = {
        std::cos(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch)),
        std::sin(glm::radians(m_Pitch)),
        std::sin(glm::radians(m_Yaw)) * std::cos(glm::radians(m_Pitch))
    };
    m_Front = glm::normalize(NewFront);
    m_Right = glm::normalize(glm::cross(m_Front, s_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    m_PlayerCam->SetDirection(m_Front, m_Up, m_Right);
    
}
