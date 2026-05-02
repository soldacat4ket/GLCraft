#pragma once

#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/TextureAtlas.h"
#include "OpenGLMinecraft/World/Block/BlockDatabase.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h"
#include "OpenGLMinecraft/World/Chunk/ChunkMesh.h"

// base unoptimized mesh generation
// will add every face of every block when generating a chunks mesh
class ChunkMeshGenerator
{
public:
    ChunkMeshGenerator();
    
    virtual ChunkMesh Consume(const Chunk& p_Chunk);
private:
    void AddWholeBlock(ChunkMesh& p_Mesh, glm::uvec3 p_BlockPos, glm::ivec3 p_ChunkPosition, const BlockData& p_BlockInfo);
};

