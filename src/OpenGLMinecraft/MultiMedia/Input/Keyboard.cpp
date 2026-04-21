#include "Keyboard.h"

#include "OpenGLMinecraft/Utility/Log.h"

Keyboard::Keyboard(const RenderWindow& p_Window)
    :m_WindowHandle(p_Window.GetWindowHandle())
{
}

Keyboard::~Keyboard()
{
}

int Keyboard::Poll(const int p_Key) const
{
    return glfwGetKey(m_WindowHandle, p_Key);
}
