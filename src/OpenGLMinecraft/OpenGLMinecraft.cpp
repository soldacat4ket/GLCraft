#include "OpenGLMinecraft.h"

#include <chrono>
#include <algorithm>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Config.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/Utility/GLMath.h"
#include "OpenGLMinecraft/World/Block/BlockDatabase.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h"
#include "OpenGLMinecraft/World/Chunk/ChunkMesh.h"
#include "OpenGLMinecraft/World/Chunk/GreedyChunkMeshGenerator.h"

OpenGLMinecraft::OpenGLMinecraft()
{
    LOG_INFO("Initializing game");
    auto StartTime = std::chrono::steady_clock::now();
    OnInit();
    auto EndTime = std::chrono::steady_clock::now();
    auto TimeToComplete = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
    LOG_DEBUG("Game initialzation took {}ms", TimeToComplete.count());
}

OpenGLMinecraft::~OpenGLMinecraft()
{
    LOG_INFO("Freeing allocated data");
    OnFree();
}

void OpenGLMinecraft::Start()
{
    LOG_INFO("Starting game loop");
    m_Running = true;
    OnStart();
    double DeltaTime = 0.0;	// Time between current frame and last frame
    double LastTime = glfwGetTime(); // Time of last frame

    do {
        m_Running = !m_RenderWindow->ShouldClose();

        double CurrentTime = glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime = CurrentTime;

        OnUpdate(DeltaTime);

        m_RenderWindow->Clear();
        OnRender();
        m_RenderWindow->Swap();
        m_RenderWindow->Poll();
    } while(m_Running);
}

void OpenGLMinecraft::Quit()
{
    m_Running = false;
}



void OpenGLMinecraft::OnInit()
{
    Config::Get().Load();
    BlockDatabase::Get().LoadBlocks(Config::Get().GetBlockDataPath());

    m_RenderWindow = std::make_unique<RenderWindow>(
        "OpenGLMinecraft", 
        Config::Get().GetGraphicsSettings().DesiredWidthPixels, 
        Config::Get().GetGraphicsSettings().DesiredHeightPixels, 
        Config::Get().GetGraphicsSettings().Fullscreen
    );
    m_RenderWindow->SetGLCapability(RenderWindow::RendererCapability::DEPTHTESTING, true);
    m_RenderWindow->SetGLCapability(RenderWindow::RendererCapability::CULLFACE, true);

    // input objects
    m_Keyboard = std::make_unique<Keyboard>(*m_RenderWindow);
    m_Mouse = std::make_unique<Mouse>(*m_RenderWindow);
    m_Camera = std::make_unique<Camera>(m_RenderWindow->GetWindowData().AspectRatioF);


    Config::Get().LoadTexturePacks();
    BlockDatabase::Get().RegisterReferenceMap(Config::Get().BuildTextureReferences());

    m_Renderer = std::make_unique<ChunkRenderer>(Config::Get().GetActivePack()->GetTexture());

    // build superflat chunk at 0,0,0 to be greedy meshed
    Chunk GreedyChunk = Chunk(glm::ivec3(0, 0, 0));
    GreedyChunk.Superflat();
    GreedyChunkMeshGenerator g;
    auto GreedyChunkMesh = g.Consume(GreedyChunk);
    m_UploadedMesh = std::make_unique<GPUMesh>(GreedyChunkMesh.GetMesh());

    // another superflat directly next to other, meshed without optimization
    Chunk NormalChunk = Chunk(glm::ivec3(0, 0, 1));
    NormalChunk.Superflat();
    ChunkMeshGenerator ug;
    auto ChunkMesh2 = ug.Consume(NormalChunk);
    m_UploadedUnoptimizedMesh = std::make_unique<GPUMesh>(ChunkMesh2.GetMesh());
}

void OpenGLMinecraft::OnFree()
{
    
}

void OpenGLMinecraft::OnStart()
{
    m_RenderWindow->SetVSync(true);
    m_RenderWindow->SetClearColor({1.0f, 0.0f, 1.0f, 1.0f});
    m_Camera->SetPos({8.0f, 8.0f, 8.0f});
    m_Mouse->RawMouseMovement();
}

void OpenGLMinecraft::OnUpdate(double DeltaTime)
{
    const Mouse::MousePos& MouseDelta = m_Mouse->GetState().MouseDelta;

    const float FlatCameraRotationAmount = 0.1f;
    const float FinalCameraRotation = FlatCameraRotationAmount * Config::Get().GetInputSettings().UserSensitivity;
    m_Camera->Rotate((float)MouseDelta.x * FinalCameraRotation, (float)MouseDelta.y * FinalCameraRotation);
    m_Camera->SetPitch(std::clamp(m_Camera->GetPitch(), -89.0f, 89.0f));
    m_Mouse->ResetDelta();


    const float CamMovementSpeed = 10.0f;
    float ScaledCamMovement = CamMovementSpeed * (float)DeltaTime;
    if(m_Keyboard->IsKeyPressed(GLFW_KEY_ESCAPE)) Quit();
    if(m_Keyboard->IsKeyPressed(GLFW_KEY_W)) m_Camera->MoveForward(ScaledCamMovement);
    if(m_Keyboard->IsKeyPressed(GLFW_KEY_S)) m_Camera->MoveBack(ScaledCamMovement);
    if(m_Keyboard->IsKeyPressed(GLFW_KEY_A)) m_Camera->MoveLeft(ScaledCamMovement);
    if(m_Keyboard->IsKeyPressed(GLFW_KEY_D)) m_Camera->MoveRight(ScaledCamMovement);
}

void OpenGLMinecraft::OnRender()
{
    m_Renderer->Begin(m_Camera.get());
    m_Renderer->Submit(m_UploadedMesh.get());
    m_Renderer->Submit(m_UploadedUnoptimizedMesh.get());
    m_Renderer->Flush();
}
