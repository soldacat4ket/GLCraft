#include "GPUMesh.h"

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/VertexBuffer.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/VertexBufferLayout.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/IndexBuffer.h"

GPUMesh::GPUMesh(const Mesh& p_Mesh)
    :m_VertexCount(p_Mesh.Data.size()), m_IndexCount(p_Mesh.Indices.size()), m_VAO(new VertexArray())
{
    // convert all mesh data to raw buffers
    const float* VertexData = reinterpret_cast<const float*>(p_Mesh.Data.data());
    const unsigned int* IndexData = reinterpret_cast<const unsigned int*>(p_Mesh.Indices.data());

    VertexBuffer DataBuffer = VertexBuffer(VertexData, m_VertexCount * sizeof(Vertex));

    VertexBufferLayout BufferLayout = VertexBufferLayout();
    BufferLayout.Push<float>(3);
    BufferLayout.Push<float>(2);

    m_VAO->AddBuffer(DataBuffer, BufferLayout);

    IndexBuffer IndiceBuffer = IndexBuffer(IndexData, m_IndexCount);
    IndiceBuffer.Bind();

    m_VAO->Unbind();
}

GPUMesh::~GPUMesh()
{
    GUARDED_DELETE(m_VAO);
}

void GPUMesh::Bind() const
{
    m_VAO->Bind();
}

void GPUMesh::Unbind() const
{
    m_VAO->Unbind();
}