#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"

// todo: add window factory or static variable to handle glfw initalization
class RenderWindow
{
public:
    typedef GLFWwindow* WindowHandle;

    RenderWindow(const char* p_Title = "Window", unsigned int p_Width = 800, unsigned int p_Height = 600, bool p_FullScreen = false, WindowHandle p_Share = NULL);
    ~RenderWindow();

    void Clear();
    void Poll();
    void Swap();

    inline bool ShouldClose() const { return glfwWindowShouldClose(m_WindowHandle); };
    inline const WindowHandle GetWindowHandle() const { return (const WindowHandle)m_WindowHandle; }

    inline void SetClearColor(glm::vec4 p_Color) { glClearColor(p_Color.x, p_Color.y, p_Color.z, p_Color.w); }
    inline void SetVSync(bool p_Enabled) { glfwSwapInterval(p_Enabled); }

    struct WindowData
    {
        char* Title;
        unsigned int Width;
        unsigned int Height;
        float AspectRatioF;
        unsigned int SimplifiedAspectRatioW;
        unsigned int SimplifiedAspectRatioH;
    };
    inline const WindowData& GetWindowData() const { return m_WindowData; }

    struct ContextData
    {
        const char* Renderer;
        const char* Vendor;
        const char* Version;
        const char* GLSLVersion;
    };
    inline const ContextData& GetContextData() { return m_ContextData; }

    enum RendererCapability
    {
        CULLFACE = GL_CULL_FACE,
        DEPTHTESTING = GL_DEPTH_TEST
    };
    void SetGLCapability(RendererCapability p_Cap, bool p_Enabled);
private:
    ContextData m_ContextData;
    WindowData m_WindowData;
    GLFWwindow* m_WindowHandle;
};

