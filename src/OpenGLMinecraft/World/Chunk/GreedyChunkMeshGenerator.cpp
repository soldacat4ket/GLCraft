#include "GreedyChunkMeshGenerator.h"

#include <chrono>

#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/World/Block/BlockIdExchanger.h"

ChunkMesh GreedyChunkMeshGenerator::Consume(const Chunk& p_Chunk)
{
    LOG_DEBUG("Building mesh for chunk {}, {}, {}", p_Chunk.GetPos().x, p_Chunk.GetPos().y, p_Chunk.GetPos().z);

    ChunkMesh Mesh(p_Chunk.GetPos());
    const Chunk::RawChunk& Blocks = p_Chunk.GetBlocks();

    auto StartTime = std::chrono::steady_clock::now();

    // for each block test if each face should be added
    GlobalBlockData::AdjacentBlocks Adjacent;
    for(unsigned int x = 0; x < CHUNK_SIZE_X; x++)
    {
        for(unsigned int y = 0; y < CHUNK_SIZE_Y; y++)
        {
            for(unsigned int z = 0; z < CHUNK_SIZE_Z; z++)
            {
                const uint16_t& Block = Blocks(x, y, z);
                if(Block == BlockDatabase::Get().Exchanger().Resolve("vanilla:air"))
                {
                    continue;
                }
                Adjacent.Of(x, y, z);
                const BlockData& Data = BlockDatabase::Get().GetData(Block);
                const glm::uvec3 Pos = {x, y, z};

                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::BottomFace, 
                    Data.TextureData.BotTexInfo,
                    Pos,
                    Adjacent.Down,
                    p_Chunk.GetPos(), 
                    Block
                );
                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::TopFace,
                    Data.TextureData.TopTexInfo,
                    Pos,
                    Adjacent.Up,
                    p_Chunk.GetPos(),
                    Block
                );
                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::LeftFace,
                    Data.TextureData.SideTexInfo,
                    Pos,
                    Adjacent.Left,
                    p_Chunk.GetPos(),
                    Block
                );
                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::RightFace,
                    Data.TextureData.SideTexInfo,
                    Pos,
                    Adjacent.Right,
                    p_Chunk.GetPos(),
                    Block
                );
                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::FrontFace,
                    Data.TextureData.SideTexInfo,
                    Pos,
                    Adjacent.Front,
                    p_Chunk.GetPos(),
                    Block
                );
                TryAddFace(
                    Mesh,
                    Blocks,
                    GlobalBlockData::BackFace,
                    Data.TextureData.SideTexInfo,
                    Pos,
                    Adjacent.Back,
                    p_Chunk.GetPos(),
                    Block
                );
            }
        }
    }

    auto EndTime = std::chrono::steady_clock::now();
    auto TimeToComplete = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
    LOG_DEBUG("Done in {}ms. Greedy mesh has {} visible faces, {} verts, and {} inds", TimeToComplete.count(), Mesh.GetFaceCount(), Mesh.GetMesh().Data.size(), Mesh.GetMesh().Indices.size());

    return Mesh;
}

void GreedyChunkMeshGenerator::TryAddFace(ChunkMesh& p_Mesh,
                                          const Chunk::RawChunk& p_Blocks,
                                          const std::array<float, 12>& p_BlockFace,
                                          const std::array<float, 8>& p_TextureCoords,
                                          const glm::uvec3 p_BlockPos,
                                          const glm::ivec3 p_AdjacentBlockPos,
                                          const glm::ivec3 p_ChunkPos,
                                          const uint16_t p_BlockId)
{
    bool IsAdjacentBlockInChunk = p_AdjacentBlockPos.x >= 0 && p_AdjacentBlockPos.x < CHUNK_SIZE_X &&
                                p_AdjacentBlockPos.y >= 0 && p_AdjacentBlockPos.y < CHUNK_SIZE_Y &&
                                p_AdjacentBlockPos.z >= 0 && p_AdjacentBlockPos.z < CHUNK_SIZE_Z;

    // add other chunk checking for blocks on edge of chunks
    if(!IsAdjacentBlockInChunk) // for now just add the faces on chunk edges for visual purposes
    {
        p_Mesh.AddFace(
            p_BlockFace,
            p_TextureCoords,
            p_ChunkPos,
            p_BlockPos
        );
        return;
    };

    // after validating adjacent block we can get its data and determine if we should make the face
    const uint16_t& AdjId = p_Blocks(p_AdjacentBlockPos.x, p_AdjacentBlockPos.y, p_AdjacentBlockPos.z);
    const bool IsAdjOpaque = BlockDatabase::Get().GetData(AdjId).IsOpaque;

    bool ShouldMakeFace = (AdjId == BlockDatabase::Get().Exchanger().Resolve("vanilla:air")) ||
                          (!IsAdjOpaque && p_BlockId != AdjId);


    if(!ShouldMakeFace) return;

    p_Mesh.AddFace(
        p_BlockFace,
        p_TextureCoords,
        p_ChunkPos,
        p_BlockPos);
}
