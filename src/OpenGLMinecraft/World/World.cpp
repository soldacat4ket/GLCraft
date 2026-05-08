#include "World.h"

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

World::World(std::unique_ptr<WorldGenerator> p_WorldGenerator, const std::filesystem::path p_WorldSaveFile, const int p_LoadDistance)
    :m_WorldSavePath(p_WorldSaveFile), m_LoadDistance(p_LoadDistance), m_Generator(std::move(p_WorldGenerator))
{
}

World::~World()
{
    for(const auto& Chunk : m_LoadedChunks)
    {
        FreeChunkData(Chunk.first);
    }
    m_LoadedChunks.clear();
}

void World::LoadNextChunk(const glm::ivec3 p_CenterChunk)
{
    for(int x = p_CenterChunk.x - m_LoadDistance; x < p_CenterChunk.x + m_LoadDistance; x++)
    {
        for(int z = p_CenterChunk.z - m_LoadDistance; z < p_CenterChunk.z + m_LoadDistance; z++)
        {
            glm::ivec3 Pos = {x, 0, z};
            bool IsLoaded = m_LoadedChunks.contains(Pos);
            if(!IsLoaded)
            {
                LoadChunk(Pos);
                return;
            }
        }
    }
}

void World::UnloadNextChunk(glm::ivec3 p_CenterChunk)
{
    // unload first chunk found of of radius
    for(const auto& [Pos, Data] : m_LoadedChunks)
    {
        bool IsInLoadRadius = (Pos.x <= p_CenterChunk.x + m_LoadDistance && Pos.x >= p_CenterChunk.x - m_LoadDistance) &&
                              (Pos.z <= p_CenterChunk.z + m_LoadDistance && Pos.z >= p_CenterChunk.z - m_LoadDistance);
        if(!IsInLoadRadius)
        {
            
            UnloadChunk(Pos);
            return;
        }
    }
}

void World::LoadChunk(const glm::ivec3 p_ChunkPosition)
{
    ChunkData d;
    d.Chunk = new Chunk(p_ChunkPosition);
    d.Chunk->WriteBlocks(m_Generator->GenerateChunk(p_ChunkPosition));
    auto Mesh = m_MeshGenerator.Consume(*d.Chunk).GetMesh();
    d.UploadedMesh = new GPUMesh(Mesh);
    m_LoadedChunks.emplace(p_ChunkPosition, d);
}

void World::UnloadChunk(const glm::ivec3 p_ChunkPosition)
{
    FreeChunkData(p_ChunkPosition);
    m_LoadedChunks.erase(p_ChunkPosition);
}

void World::FreeChunkData(const glm::ivec3 p_Position)
{
    ASSERT(m_LoadedChunks.contains(p_Position));

    auto& Data = m_LoadedChunks.at(p_Position);
    GUARDED_DELETE(Data.Chunk);
    GUARDED_DELETE(Data.UploadedMesh);
}

void World::WriteChunkToDisk(const glm::ivec3 p_Position, std::filesystem::path p_SavePath)
{
    ASSERT(m_LoadedChunks.contains(p_Position));
}
