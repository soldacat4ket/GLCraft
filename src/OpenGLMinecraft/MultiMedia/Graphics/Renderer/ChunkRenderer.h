#pragma once

#include <queue>

#include "OpenGLMinecraft/MultiMedia/Graphics/Camera.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/GPUMesh.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Shaders/Shader.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/Texture.h"

#define DEFAULT_CHUNK_VERT std::filesystem::current_path() / "res" / "shaders" / "chunkvert.glsl"
#define DEFAULT_CHUNK_FRAG std::filesystem::current_path() / "res" / "shaders" / "chunkfrag.glsl"

class ChunkRenderer
{
public:
    // shader of nullptr will cause the renderer to manage its own shader object using the default macros
    ChunkRenderer(const Texture* p_AtlasImage, const Shader* p_ChunkShader = nullptr);
    ~ChunkRenderer();

    void Begin(const Camera* p_Cam, bool p_Wireframe = false);
    void Submit(const GPUMesh* p_ChunkMesh);
    void Flush();
private:
    void Render(const GPUMesh* p_ChunkMesh);

    bool m_Status;
    bool m_Wireframe;
    bool m_ManageDefaultShader;

    const Shader* m_Shader;
    const Camera* m_Cam;
    const Texture* m_AtlasImage;
    
    // potentially change to priority queue for back to front transparency rendering
    std::queue<const GPUMesh*> m_CommandPool;
};