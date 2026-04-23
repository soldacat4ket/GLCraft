#include "RayCast.h"

RayCast::RayCast(const glm::vec3 p_Position, const glm::vec3 p_Direction)
    : m_RayStart(p_Position), m_RayEnd(p_Position), m_RayDirection(p_Direction)
{

}

void RayCast::Step(float p_Amount)
{
    float Yaw = glm::radians(m_RayDirection.y + 90.0f);
    float Pitch = glm::radians(m_RayDirection.x);

    m_RayEnd -= glm::cos(Yaw) * p_Amount;
    m_RayEnd -= glm::sin(Yaw) * p_Amount;
    m_RayEnd -= glm::tan(Pitch) * p_Amount;
}
