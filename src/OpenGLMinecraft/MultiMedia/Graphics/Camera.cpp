#include "Camera.h"

Camera::Camera(
    const float p_AspectRatio, 
    const float p_Near, 
    const float p_Far, 
    const float p_Fov, 
    const glm::vec3 p_Position, 
    const glm::vec3 p_Up, 
    const glm::vec3 p_Front
)
    : m_AspectRatio(p_AspectRatio), m_Near(p_Near), m_Far(p_Far), m_Fov(p_Fov), m_Position(p_Position), m_WorldUp(p_Up), m_Front(p_Front)
{
}