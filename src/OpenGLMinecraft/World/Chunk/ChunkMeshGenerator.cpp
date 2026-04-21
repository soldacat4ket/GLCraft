#include "ChunkMeshGenerator.h"

#include <type_traits>
#include <chrono>

#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/World/Block/BlockIdExchanger.h"

ChunkMeshGenerator::ChunkMeshGenerator()
{
}

ChunkMesh ChunkMeshGenerator::Consume(const Chunk& p_Chunk)
{
    LOG_DEBUG("Building mesh for chunk {}, {}, {}", p_Chunk.GetPos().x, p_Chunk.GetPos().y, p_Chunk.GetPos().z);


    ChunkMesh Mesh(p_Chunk.GetPos());
    const Chunk::RawChunk& Blocks = p_Chunk.GetBlocks();

    auto StartTime = std::chrono::steady_clock::now();

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
                const BlockData& Data = BlockDatabase::Get().GetData(Block);
                const glm::uvec3 Pos = {x, y, z};
                AddWholeBlock(Mesh, Pos, p_Chunk.GetPos(), Data);
            }
        }
    }

    auto EndTime = std::chrono::steady_clock::now();
    auto TimeToComplete = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);
    LOG_DEBUG("Done in {}ms. Unoptimized mesh has {} visible blocks, {} verts, and {} inds", TimeToComplete.count(), Mesh.GetFaceCount() / 6, Mesh.GetMesh().Data.size(), Mesh.GetMesh().Indices.size());

    return Mesh;
}

void ChunkMeshGenerator::AddWholeBlock(ChunkMesh& p_Mesh, const glm::uvec3 p_BlockPos, const glm::ivec3 p_ChunkPos, const BlockData& p_BlockInfo)
{
    p_Mesh.AddFace(
        GlobalBlockData::BottomFace,
        p_BlockInfo.TextureData.BotTexInfo,
        p_ChunkPos,
        p_BlockPos);

    p_Mesh.AddFace(
        GlobalBlockData::TopFace,
        p_BlockInfo.TextureData.TopTexInfo,
        p_ChunkPos,
        p_BlockPos);

    p_Mesh.AddFace(
        GlobalBlockData::LeftFace,
        p_BlockInfo.TextureData.SideTexInfo,
        p_ChunkPos,
        p_BlockPos);

    p_Mesh.AddFace(
        GlobalBlockData::RightFace,
        p_BlockInfo.TextureData.SideTexInfo,
        p_ChunkPos,
        p_BlockPos);

    p_Mesh.AddFace(
        GlobalBlockData::BackFace,
        p_BlockInfo.TextureData.SideTexInfo,
        p_ChunkPos,
        p_BlockPos);

    p_Mesh.AddFace(
        GlobalBlockData::FrontFace,
        p_BlockInfo.TextureData.SideTexInfo,
        p_ChunkPos,
        p_BlockPos);
}
