#include "ChunkMesh.h"

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h" // for chunk size macros only

ChunkMesh::ChunkMesh(const glm::ivec3& p_ChunkPosition)
    :m_Position(p_ChunkPosition), m_FaceCounter(0), m_IndiceValue(0)
{
}

ChunkMesh::~ChunkMesh()
{
}

void ChunkMesh::AddFace(
    const std::array<float, 12>& p_BlockFace, 
    const std::array<float, 8>& p_TexCoords,
    const glm::ivec3 p_ChunkPosition,
    const glm::uvec3 p_BlockPosition)
{
    m_FaceCounter++;

    // classic floating point precision bs, chunks very far from the origin store very large floats for their verts
    // fix? idfk, maybe send position data to shaders
    const glm::vec3 BlockOffset = {
        p_ChunkPosition.x * CHUNK_SIZE_X + p_BlockPosition.x,
        p_ChunkPosition.y * CHUNK_SIZE_Y + p_BlockPosition.y,
        p_ChunkPosition.z * CHUNK_SIZE_Z + p_BlockPosition.z
    };

    // group the vertex data by 3 and texture data by 2, adding the common demoninator of 4 vertices
    std::array<float, 12>::const_iterator VertexIterator = p_BlockFace.begin();
    std::array<float, 8>::const_iterator TextureIterator = p_TexCoords.begin();
    for(int i = 0; i < 4; i++)
    {
        m_Mesh.Data.push_back({
            { // vertex positions
                (*VertexIterator++) + BlockOffset.x,
                (*VertexIterator++) + BlockOffset.y,
                (*VertexIterator++) + BlockOffset.z
            },
            { // vertex textures
                (*TextureIterator++),
                (*TextureIterator++)
            }
        });
    }

    // m_IndiceValue is the index of the bottom left
    // + 1 is the index of the bottom right
    // + 2 is the index of the top right
    // + 3 is the index of the top left
    // in the below order indices draw bottom left, bottom right, top right, top right top left, bottom left
    const std::array<unsigned int, 6> m_FaceIndices = {
        m_IndiceValue, 
        m_IndiceValue + 1, 
        m_IndiceValue + 2, 
        m_IndiceValue + 2, 
        m_IndiceValue + 3, 
        m_IndiceValue
    };

    m_Mesh.Indices.insert(m_Mesh.Indices.end(), m_FaceIndices.begin(), m_FaceIndices.end());

    // move the indice value to match the next incoming vertex data
    m_IndiceValue += 4;
}
