#include "RenderWindow.h"

#include <numeric>

#include "OpenGLMinecraft/Utility/Log.h"

static bool s_GLFWInitialized = false;

RenderWindow::RenderWindow(const char* p_Title, const unsigned int p_Width, const unsigned int p_Height, const bool p_FullScreen, WindowHandle p_Share)
{
    if(!s_GLFWInitialized)
    {
        s_GLFWInitialized = true;
        glfwInit();
    }
    // create window and load opengl context
    m_WindowHandle = glfwCreateWindow(
        p_Width, 
        p_Height, 
        p_Title, 
        p_FullScreen ? glfwGetPrimaryMonitor() : NULL, 
        p_Share
    );
    glfwMakeContextCurrent(m_WindowHandle);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    m_WindowData.Title = (char*)p_Title;
    m_WindowData.Width = p_Width;
    m_WindowData.Height = p_Height;
    m_WindowData.AspectRatioF = (float)p_Width / p_Height;

    auto Common = std::gcd(p_Width, p_Height); // calculate simplified aspect ratio float = 1.3333 and simplified fraction = 4/3
    m_WindowData.SimplifiedAspectRatioW = p_Width / Common;
    m_WindowData.SimplifiedAspectRatioH = p_Height / Common;

    LOG_INFO("New window \"{}\" -> W: {} H: {} ARF: {} AR:{}/{}", m_WindowData.Title, m_WindowData.Width, m_WindowData.Height, m_WindowData.AspectRatioF, m_WindowData.SimplifiedAspectRatioW, m_WindowData.SimplifiedAspectRatioH);

    m_ContextData.Renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    m_ContextData.Vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    m_ContextData.Version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    m_ContextData.GLSLVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    LOG_INFO("New context \"{}\", Vendor: {} OpenGL Version: {} Shader Version: {}", m_ContextData.Renderer, m_ContextData.Vendor, m_ContextData.Version, m_ContextData.GLSLVersion);
}

RenderWindow::~RenderWindow()
{
    LOG_INFO("Destroying window {}", m_WindowData.Title);
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
}

void RenderWindow::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderWindow::Poll()
{
    glfwPollEvents();
}

void RenderWindow::Swap()
{
    glfwSwapBuffers(m_WindowHandle);
}

void RenderWindow::SetGLCapability(const RendererCapability p_Cap, const bool p_Enabled)
{
    p_Enabled ? glEnable(p_Cap) : glDisable(p_Cap);
}