#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Camera.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Renderer/ChunkRenderer.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Renderer/ChunkRenderer.h"
#include "OpenGLMinecraft/MultiMedia/Input/Keyboard.h"
#include "OpenGLMinecraft/MultiMedia/Input/Mouse.h"

class OpenGLMinecraft
{
public:
    OpenGLMinecraft();
    ~OpenGLMinecraft();

    // start main application loop
    void Start();

    void Quit();
protected:
    void OnInit();
    void OnStart();
    void OnUpdate(double DeltaTime);
    void OnRender();
    void OnFree();

    bool m_Running;

    std::unique_ptr<RenderWindow> m_RenderWindow;
    std::unique_ptr<Camera> m_Camera;
    std::unique_ptr<ChunkRenderer> m_Renderer;
    std::unique_ptr<Keyboard> m_Keyboard;
    std::unique_ptr<Mouse> m_Mouse;
    std::unique_ptr<GPUMesh> m_UploadedMesh;
    std::unique_ptr<GPUMesh> m_UploadedUnoptimizedMesh;
};

