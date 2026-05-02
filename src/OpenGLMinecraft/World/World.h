#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include <glm/gtx/hash.hpp>

#include "OpenGLMinecraft/World/WorldGeneration/WorldGenerator.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h"

// curent implementation is ass, consider having the world take in load distance and update with player position
// lots of potential optimization here, def gonna have to look into it 
class World
{
public:
    World(std::unique_ptr<WorldGenerator> p_WorldGenerator, std::filesystem::path p_WorldSavePath, unsigned int p_LoadDistance);
    ~World();

    // get a chunk, load the chunk if not already
    const Chunk& GetChunk(glm::ivec3 p_ChunkPosition);

    void UnloadChunk();
    void SaveSingleChunkToDisk(glm::ivec3 p_ChunkPosition);

private:
    std::unique_ptr<WorldGenerator> m_Generator;
    std::filesystem::path m_WorldSavePath;

    std::unordered_map<glm::ivec3, Chunk*> m_LoadedChunks;
};

