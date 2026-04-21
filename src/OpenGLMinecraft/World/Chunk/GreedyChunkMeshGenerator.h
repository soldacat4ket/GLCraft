#pragma once

#include "OpenGLMinecraft/World/Chunk/ChunkMeshGenerator.h"

class GreedyChunkMeshGenerator : ChunkMeshGenerator
{
public:
    using ChunkMeshGenerator::ChunkMeshGenerator;

    // builds a chunk mesh but only adds faces if they are visible
    virtual ChunkMesh Consume(const Chunk& p_Chunk) override;
private:

    // determine if a face should be added and add it to the mesh if so
    void TryAddFace(
        ChunkMesh& p_Mesh,
        const Chunk::RawChunk& p_Blocks,
        const std::array<float, 12>& p_BlockFace,
        const std::array<float, 8>& p_TextureCoords,
        glm::uvec3 p_BlockPos,
        glm::ivec3 p_AdjacentBlockPos,
        glm::ivec3 p_ChunkPos,
        uint16_t p_BlockId
    );
};

