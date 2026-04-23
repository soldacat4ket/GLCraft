#pragma once

#include <glm/glm.hpp>

class RayCast
{
public:
    RayCast(glm::vec3 p_Position, glm::vec3 p_Direction);

    void Step(float p_Amount);

    inline glm::vec3 End() const { return m_RayEnd; }
    float Length() const { return glm::distance(m_RayStart, m_RayEnd); }
private:
    glm::vec3 m_RayStart;
    glm::vec3 m_RayDirection;
    glm::vec3 m_RayEnd;
};