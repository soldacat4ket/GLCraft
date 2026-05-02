#pragma once

#include <queue>

#include "OpenGLMinecraft/MultiMedia/Graphics/Camera.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/GPUMesh.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Shader.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/Texture.h"

#define DEFAULT_CHUNK_VERT std::filesystem::current_path() / "res" / "shaders" / "chunkvert.glsl"
#define DEFAULT_CHUNK_FRAG std::filesystem::current_path() / "res" / "shaders" / "chunkfrag.glsl"

class ChunkRenderer
{
public:
    ChunkRenderer(const Texture* p_AtlasImage);

    void Begin(glm::mat4 p_ViewProjectionMatrix, bool p_Wireframe = false);
    void Submit(const GPUMesh* p_ChunkMesh, const Shader* p_ChunkShader);
    void Flush();
private:

    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
        const Texture* AtlasImage;
    };

    SceneData m_SceneData;

    bool m_Status;
    bool m_Wireframe;
};