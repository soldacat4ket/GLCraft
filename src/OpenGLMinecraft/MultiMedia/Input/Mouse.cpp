#include "Mouse.h"

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

Mouse::Mouse(const RenderWindow& p_Window)
    :m_WindowHandle(p_Window.GetWindowHandle()), m_Locked(false), m_RawMouseEnabled(false)
{
    glfwSetWindowUserPointer(m_WindowHandle, &m_State);

    // callback for every time the mouse is moved
    glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* p_Window, double XPos, double YPos)
    {
        MouseState* s = static_cast<MouseState*>(glfwGetWindowUserPointer(p_Window));

        if(!std::isfinite(XPos) || !std::isfinite(YPos)) return; // throw out garbage values before theyre passed

        const MousePos CurentPos = {XPos, YPos};
        s->MouseDelta = CurentPos.YFlipSubtract(s->LastPos);
        s->LastPos = CurentPos;
    });
}

Mouse::~Mouse()
{
}

void Mouse::Lock(const bool p_Locked)
{
    m_Locked = p_Locked;
    LOG_DEBUG("Mouse is {}", p_Locked ? "Locked" : "Unlocked");
    glfwSetInputMode(m_WindowHandle, GLFW_CURSOR, p_Locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Mouse::RawMouseMovement(const bool p_Enabled)
{
    ASSERT(glfwRawMouseMotionSupported() && p_Enabled);
    Lock();
    m_RawMouseEnabled = p_Enabled;
    glfwSetInputMode(m_WindowHandle, GLFW_RAW_MOUSE_MOTION, p_Enabled ? GLFW_TRUE : GLFW_FALSE);
}

Mouse::MousePos Mouse::GetPos() const
{
    MousePos p;
    glfwGetCursorPos(m_WindowHandle, &p.x, &p.y);
    return p;
}
