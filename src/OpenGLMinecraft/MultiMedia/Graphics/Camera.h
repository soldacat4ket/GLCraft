#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(
        float p_AspectRatio, 
        float p_Near = 0.1f,
        float p_Far = 100.0f,
        float p_Fov = 45.0f,
        glm::vec3 p_Position = {0.0f, 0.0f, 0.0f},
        glm::vec3 p_Up = {0.0f, 1.0f, 0.0f}, 
        glm::vec3 p_Front = {0.0f, 0.0f, -1.0f}
    );

    inline void SetDirection(glm::vec3 p_NewFront, glm::vec3 p_NewUp, glm::vec3 p_NewRight) { m_Front = p_NewFront; m_Up = p_NewUp; m_Right = p_NewRight; }

    inline void SetPosition(glm::vec3 p_Position) { m_Position = p_Position; }

    inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
    inline glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far); }
    inline glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }
private:

    glm::vec3 m_Position;
    glm::vec3 m_Front, m_Up, m_Right;
    glm::vec3 m_WorldUp;

    float m_AspectRatio;
    float m_Fov;
    float m_Near;
    float m_Far;
};

