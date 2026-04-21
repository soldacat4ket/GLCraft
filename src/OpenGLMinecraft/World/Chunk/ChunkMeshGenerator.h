#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/TextureAtlas.h"
#include "OpenGLMinecraft/World/Block/BlockDatabase.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h"
#include "OpenGLMinecraft/World/Chunk/ChunkMesh.h"

// base unoptimized mesh generation
class ChunkMeshGenerator
{
public:
    ChunkMeshGenerator();
    
    // builds a chunk mesh by adding every face of every block from the chunk
    virtual ChunkMesh Consume(const Chunk& p_Chunk);
private:
    // add every face of a block to the mesh
    void AddWholeBlock(ChunkMesh& p_Mesh, glm::uvec3 p_BlockPos, glm::ivec3 p_ChunkPosition, const BlockData& p_BlockInfo);
};

