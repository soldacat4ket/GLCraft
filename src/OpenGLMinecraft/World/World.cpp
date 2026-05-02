#include "World.h"

#include "OpenGLMinecraft/Debug.h"

World::World(std::unique_ptr<WorldGenerator> p_WorldGenerator, const std::filesystem::path p_WorldSaveFile, const unsigned int p_LoadDistance)
    :m_WorldSavePath(p_WorldSaveFile)
{
    m_Generator = std::move(p_WorldGenerator);
}

World::~World()
{
    for(const auto& [Pos, Chunk] : m_LoadedChunks)
    {
        GUARDED_DELETE(Chunk);
    }
}

const Chunk& World::GetChunk(const glm::ivec3 p_ChunkPosition)
{
    if(!m_LoadedChunks.contains(p_ChunkPosition))
    {
        m_LoadedChunks.emplace(p_ChunkPosition, m_Generator->GenerateChunk(p_ChunkPosition));
    }
    return *m_LoadedChunks[p_ChunkPosition];
}
