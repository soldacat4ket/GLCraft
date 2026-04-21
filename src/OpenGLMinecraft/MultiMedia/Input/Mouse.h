#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"

// https://www.glfw.org/docs/latest/input_guide.html

class Mouse
{
public:
    struct MousePos
    {
        double x, y;

        MousePos operator-(const MousePos& p) const
        {
            return {x - p.x, y - p.y};
        }
        MousePos operator*(const float p) const
        {
            return {x * p, y * p};
        }
        MousePos YFlipSubtract(const MousePos& p) const // subtract mousepos values since glfw stores y coords in the opposite direction, rarely used by game code
        {
            return {x - p.x, p.y - y};
        }
    };

    struct MouseState
    {
        MousePos LastPos = {0.0, 0.0};
        MousePos MouseDelta = {0.0, 0.0};
    };

    Mouse(const RenderWindow& p_Window);
    ~Mouse();

    void Lock(bool p_Locked = true);
    void RawMouseMovement(bool p_Enabled = true);
    inline bool IsLocked() const { return m_Locked; }
    inline const MouseState& GetState() const { return m_State; }
    inline void ResetDelta() { m_State.MouseDelta = {0.0, 0.0}; }
    MousePos GetPos() const;
private:
    RenderWindow::WindowHandle m_WindowHandle;
    bool m_RawMouseEnabled;
    bool m_Locked;

    MouseState m_State;
};

