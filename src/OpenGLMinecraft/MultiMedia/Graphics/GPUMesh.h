#pragma once

#include "OpenGLMinecraft/Utility/NonCopyable.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Mesh.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/VertexArray.h"

class GPUMesh : NonCopyable
{
public:
    GPUMesh(const Mesh& p_Mesh);
    ~GPUMesh();

    void Bind() const;
    void Unbind() const;

    inline size_t GetVertexCount() const { return m_VertexCount; }
    inline size_t GetIndexCount() const { return m_IndexCount; }
private:
    size_t m_VertexCount;
    size_t m_IndexCount;

    VertexArray* m_VAO;
};