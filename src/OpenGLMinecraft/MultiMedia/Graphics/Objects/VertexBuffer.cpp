#include "VertexBuffer.h"

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"

VertexBuffer::VertexBuffer(const void* p_Data, const size_t p_SizeBytes) 
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, p_SizeBytes, p_Data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() 
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::	Unbind() const 
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}