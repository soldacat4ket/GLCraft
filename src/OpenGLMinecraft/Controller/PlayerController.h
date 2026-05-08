#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/MultiMedia/Input/Keyboard.h"
#include "OpenGLMinecraft/MultiMedia/Input/Mouse.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Camera.h"

class PlayerController
{
public:
    virtual void HandleInput(const Mouse* p_Mouse, const Keyboard* p_Keyboard, double p_DeltaTime) = 0;

    inline glm::vec3 GetPosition() { return m_Position; }
    inline const Camera* GetCamera() { return m_PlayerCam.get(); };
protected:
    glm::vec3 m_Position;
    std::unique_ptr<Camera> m_PlayerCam;
};

