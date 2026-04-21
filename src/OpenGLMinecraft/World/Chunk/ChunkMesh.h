#pragma once

#include <memory>
#include <array>
#include <glm/glm.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/Objects/Mesh.h"

class ChunkMesh
{
public:
    ChunkMesh(const glm::ivec3& p_ChunkPosition);
    ~ChunkMesh();

    // offset and add face to the mesh
    void AddFace(
        const std::array<float, 12>& p_BlockFace,
        const std::array<float, 8>& p_TexCoords,
        glm::ivec3 p_ChunkPosition,
        glm::uvec3 p_BlockPosition
    );

    inline const Mesh& GetMesh() const { return m_Mesh; }
    inline glm::ivec3 GetPos() const { return m_Position; }
    inline unsigned int GetFaceCount() const { return m_FaceCounter; }
private:
    const glm::ivec3 m_Position; // position used to calculate final vertex data
    Mesh m_Mesh;
    unsigned int m_FaceCounter;
    unsigned int m_IndiceValue;
};