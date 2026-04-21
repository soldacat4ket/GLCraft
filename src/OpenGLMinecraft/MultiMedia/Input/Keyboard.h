#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"

class Keyboard
{
public:
    Keyboard(const RenderWindow& p_Window);
    ~Keyboard();

    int Poll(int p_Key) const;

    inline bool IsKeyPressed(int p_Key) const { return Poll(p_Key) == GLFW_PRESS; };
private:
    RenderWindow::WindowHandle m_WindowHandle;
};