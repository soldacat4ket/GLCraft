#include "IndexBuffer.h"

#include "OpenGLMinecraft/MultiMedia/Graphics/GLCommon.h"
#include "OpenGLMinecraft/Debug.h"

IndexBuffer::IndexBuffer(const unsigned int* p_Data, const size_t p_IndexCount)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_IndexCount * sizeof(unsigned int), p_Data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() 
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const 
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}