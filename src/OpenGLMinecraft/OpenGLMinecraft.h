#pragma once

#include <memory>

#include "OpenGLMinecraft/Controller/FreeCamPlayerController.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/RenderWindow.h"
#include "OpenGLMinecraft/World/World.h"
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

    void LoadTestChunks(); // populates raw GPUMesh with test chunk data
    void SubmitTestChunks(); // submites all test chunks to the renderer
    std::unique_ptr<GPUMesh> m_UploadedMesh;
    std::unique_ptr<GPUMesh> m_UploadedUnoptimizedMesh;
    std::unique_ptr<GPUMesh> m_CustomGeneratedMesh;
    std::unique_ptr<GPUMesh> m_CustomSphereMesh;

    bool m_Running;
    double m_FPS;

    std::unique_ptr<RenderWindow> m_RenderWindow;
    std::unique_ptr<ChunkRenderer> m_Renderer;

    std::unique_ptr<Shader> m_SolidShader;


    std::unique_ptr<Keyboard> m_Keyboard;
    std::unique_ptr<Mouse> m_Mouse;

    std::unique_ptr<PlayerController> m_Player;

    std::unique_ptr<World> m_World;

};

