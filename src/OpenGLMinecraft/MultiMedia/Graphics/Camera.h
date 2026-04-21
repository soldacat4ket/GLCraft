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
        glm::vec3 p_Front = {0.0f, 0.0f, -1.0f},
        float p_Yaw = -90.0f, 
        float p_Pitch = 0.0f
    );

    // rotation methods
    inline void Rotate(float p_YawOffset, float p_PitchOffset) { m_Yaw += p_YawOffset; m_Pitch += p_PitchOffset; UpdateCameraVectors(); }
    inline void SetRotation(float p_Yaw, float p_Pitch) { m_Yaw = p_Yaw; m_Pitch = p_Pitch; UpdateCameraVectors(); }
    inline void SetPitch(float p_Pitch) { m_Pitch = p_Pitch; UpdateCameraVectors(); }
    inline void SetYaw(float p_Yaw) { m_Yaw = p_Yaw; UpdateCameraVectors(); }

    // movemnt methods
    inline void Move(glm::vec3 p_PositionOffset) { m_Position += p_PositionOffset; }
    inline void SetPos(glm::vec3 p_Position) { m_Position = p_Position; }
    inline void MoveForward(float p_Amount) { m_Position += m_Front * p_Amount; }
    inline void MoveBack(float p_Amount) { m_Position -= m_Front * p_Amount; }
    inline void MoveLeft(float p_Amount) { m_Position -= m_Right * p_Amount; }
    inline void MoveRight(float p_Amount) { m_Position += m_Right * p_Amount; }

    // getters
    inline float GetYaw() const { return m_Yaw; }
    inline float GetPitch() const { return m_Pitch; }

    // matrix getters
    inline glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }
    inline glm::mat4 GetProjectionMatrix() const { return glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far); }
    inline glm::mat4 GetViewProjectionMatrix() const { return GetProjectionMatrix() * GetViewMatrix(); }
private:
    void UpdateCameraVectors();

    glm::vec3 m_Position;
    glm::vec3 m_Front, m_Up, m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw, m_Pitch;

    float m_AspectRatio;
    float m_Fov;
    float m_Near;
    float m_Far;
};

