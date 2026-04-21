#include "ChunkRenderer.h"

#include <glm/gtx/string_cast.hpp>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Renderer/RenderCommands.h"

ChunkRenderer::ChunkRenderer(const Texture* p_AtlasImage, const Shader* p_ChunkShader)
    :m_AtlasImage(p_AtlasImage), m_Shader(p_ChunkShader), m_Cam(nullptr), m_Status(false), m_Wireframe(false), m_ManageDefaultShader(!p_ChunkShader)
{
    if(m_ManageDefaultShader) m_Shader = new Shader(DEFAULT_CHUNK_VERT, DEFAULT_CHUNK_FRAG);
}

ChunkRenderer::~ChunkRenderer()
{
    if(m_ManageDefaultShader) GUARDED_DELETE(m_Shader);
}

void ChunkRenderer::Begin(const Camera* p_Cam, bool p_Wireframe)
{
    m_Cam = p_Cam;
    m_Status = true;
    m_Wireframe = p_Wireframe;
}

void ChunkRenderer::Submit(const GPUMesh* c)
{
    m_CommandPool.push(c);
}

void ChunkRenderer::Render(const GPUMesh* c)
{
    auto proj = m_Cam->GetViewProjectionMatrix();
    m_Shader->SetMat4("u_MVP", proj);

    c->Bind();
    RenderCommands::DrawElements(static_cast<GLsizei>(c->GetIndexCount()));

}

void ChunkRenderer::Flush()
{
    auto DrawMode = m_Wireframe ? GL_LINE : GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, DrawMode);

    m_Shader->Bind();
    m_Shader->SetSampler2D("u_Tex", *m_AtlasImage);

    while(!m_CommandPool.empty())
    {
        Render(m_CommandPool.front());
        m_CommandPool.pop();
    }
}