#include "Camera.h"

Camera::Camera(
    const float p_AspectRatio, 
    const float p_Near, 
    const float p_Far, 
    const float p_Fov, 
    const glm::vec3 p_Position, 
    const glm::vec3 p_Up, 
    const glm::vec3 p_Front,
    const float p_Yaw,
    const float p_Pitch
)
    : m_AspectRatio(p_AspectRatio), m_Near(p_Near), m_Far(p_Far), m_Fov(p_Fov), m_Position(p_Position), m_WorldUp(p_Up), m_Yaw(p_Yaw), m_Pitch(p_Pitch)
{
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    const glm::vec3 NewFront = {
        cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)),
        sin(glm::radians(m_Pitch)),
        sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch))
    };
    m_Front = glm::normalize(NewFront);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
