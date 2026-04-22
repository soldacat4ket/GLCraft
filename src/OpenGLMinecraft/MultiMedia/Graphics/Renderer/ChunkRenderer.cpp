#include "ChunkRenderer.h"

#include <glm/gtx/string_cast.hpp>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Renderer/RenderCommands.h"

ChunkRenderer::ChunkRenderer(const Texture* p_AtlasImage)
    :m_Status(false), m_Wireframe(false)
{
    m_SceneData.AtlasImage = p_AtlasImage;
}

void ChunkRenderer::Begin(const Camera* p_Cam, bool p_Wireframe)
{
    m_SceneData.ViewProjectionMatrix = p_Cam->GetViewProjectionMatrix();
    m_Status = true;
    m_Wireframe = p_Wireframe;
    auto DrawMode = m_Wireframe ? GL_LINE : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, DrawMode);
}

void ChunkRenderer::Submit(const GPUMesh* c, const Shader* p_ChunkShader)
{
    p_ChunkShader->Bind();
    p_ChunkShader->SetSampler2D("u_Tex", *m_SceneData.AtlasImage);

    p_ChunkShader->SetMat4("u_MVP", m_SceneData.ViewProjectionMatrix);

    c->Bind();

    RenderCommands::DrawElements(static_cast<GLsizei>(c->GetIndexCount()));
}

void ChunkRenderer::Flush()
{
   
}