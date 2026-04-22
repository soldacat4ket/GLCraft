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
    BlockDatabase::Get().LoadBlocks(Config::Get().GetBlockDataFile());

    m_RenderWindow = std::make_unique<RenderWindow>(
        "OpenGLMinecraft", 
        Config::Get().GetGraphicsSettings().DesiredWidthPixels, 
        Config::Get().GetGraphicsSettings().DesiredHeightPixels, 
        Config::Get().GetGraphicsSettings().Fullscreen
    );
    m_RenderWindow->SetGLCapability(RenderWindow::RendererCapability::DEPTHTESTING, true);
    #ifdef NDEBUG
    m_RenderWindow->SetGLCapability(RenderWindow::RendererCapability::CULLFACE, true);
    #endif
    // input objects
    m_Keyboard = std::make_unique<Keyboard>(*m_RenderWindow);
    m_Mouse = std::make_unique<Mouse>(*m_RenderWindow);
    m_Camera = std::make_unique<Camera>(m_RenderWindow->GetWindowData().AspectRatioF);

    m_SolidShader = std::make_unique<Shader>(Config::Get().GetGraphicsSettings().SolidVertexShaderFile, Config::Get().GetGraphicsSettings().SolidFragmentShaderFile);

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

    // test out the new .GenerateCustom() using a lambda, horribly nested but it should explain how we can generate chunks
    Chunk CustomChunk = Chunk(glm::ivec3(1, 0, 0));
    CustomChunk.GenerateCustom(
        [](Chunk::RawChunk& p_Blocks)
        {
            // for each column on the x and z axis
            for(int x = 0; x < p_Blocks.SizeX(); x++)
            {
                for(int z = 0; z < p_Blocks.SizeZ(); z++)
                {
                    // generate a height from the x and z where the shortest is 0
                    const int ChunkHeight = (x / 2) + (x % 2) + (z / 2) + (z % 2);
                    for(int y = 0; y < ChunkHeight; y++)
                    {
                        uint16_t BlockType;

                        // depending on the blocks position, generate a bedrock on the bottom, fill with stone, add 2 dirt, and top with grass
                        if(y >= ChunkHeight - 1)
                        {
                            BlockType = BlockDatabase::Get().Exchanger().Resolve("vanilla:grass_block");
                        }
                        else if(y >= ChunkHeight - 3)
                        {
                            BlockType = BlockDatabase::Get().Exchanger().Resolve("vanilla:dirt_block");
                        }
                        else if(y > 0)
                        {
                            BlockType = BlockDatabase::Get().Exchanger().Resolve("vanilla:stone_block");
                        }
                        else
                        {
                            BlockType = BlockDatabase::Get().Exchanger().Resolve("vanilla:bedrock_block");
                        }
                        p_Blocks(x, y, z) = BlockType;
                    }
                }
            }
        }
    );
    auto CustomMesh = g.Consume(CustomChunk);
    m_CustomGeneratedMesh = std::make_unique<GPUMesh>(CustomMesh.GetMesh());

    Chunk CustomSphereChunk = Chunk(glm::ivec3(1, 0, 1));
    CustomSphereChunk.GenerateCustom(
        [](Chunk::RawChunk& p_Blocks)
        {
            const glm::ivec3 Center = {p_Blocks.SizeX() / 2, p_Blocks.SizeY() / 2, p_Blocks.SizeZ() / 2};
            const int Radius = 6;

            for(int x = 0; x < p_Blocks.SizeX(); x++)
            {
                for(int y = 0; y < p_Blocks.SizeY(); y++)
                {
                    for(int z = 0; z < p_Blocks.SizeZ(); z++)
                    {
                        //distance to sphere
                        int Distance = std::pow(x - Center.x, 2) + std::pow(y - Center.y, 2) + std::pow(z - Center.z, 2);
                        if(Distance <= std::pow(Radius, 2))
                        {
                            p_Blocks(x, y, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:bedrock_block");
                        }
                    }
                }
            }
        }
    );

    auto SphereMesh = g.Consume(CustomSphereChunk);
    m_CustomSphereMesh = std::make_unique<GPUMesh>(SphereMesh.GetMesh());
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
    m_Renderer->Submit(m_UploadedMesh.get(), m_SolidShader.get());
    m_Renderer->Submit(m_UploadedUnoptimizedMesh.get(), m_SolidShader.get());
    m_Renderer->Submit(m_CustomGeneratedMesh.get(), m_SolidShader.get());
    m_Renderer->Submit(m_CustomSphereMesh.get(), m_SolidShader.get());
    m_Renderer->Flush();
}
