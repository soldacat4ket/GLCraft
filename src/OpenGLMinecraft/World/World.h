#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/GPUMesh.h"
#include "OpenGLMinecraft/World/WorldGeneration/WorldGenerator.h"
#include "OpenGLMinecraft/World/Chunk/GreedyChunkMeshGenerator.h"
#include "OpenGLMinecraft/World/Chunk/Chunk.h"

// curent implementation is ass, consider having the world take in load distance and update with player position
// lots of potential optimization here, def gonna have to look into it 
class World
{
public:
    struct ChunkData
    {
        Chunk* Chunk;
        GPUMesh* UploadedMesh;
    };

    World(std::unique_ptr<WorldGenerator> p_WorldGenerator, std::filesystem::path p_WorldSavePath, int p_LoadDistance);
    ~World();

    void LoadNextChunk(glm::ivec3 p_CenterChunk);
    void UnloadNextChunk(glm::ivec3 p_CenterChunk);

    inline const std::unordered_map<glm::ivec3, ChunkData>& GetChunkData() const { return m_LoadedChunks; };
private:
    void LoadChunk(glm::ivec3 p_ChunkPosition);
    void UnloadChunk(glm::ivec3 p_ChunkPosition);
    void FreeChunkData(glm::ivec3 p_ChunkPosition);
    void WriteChunkToDisk(glm::ivec3 p_Chunk, std::filesystem::path p_SavePath);

    std::unique_ptr<WorldGenerator> m_Generator;
    int m_LoadDistance;
    std::filesystem::path m_WorldSavePath;
    GreedyChunkMeshGenerator m_MeshGenerator;
    std::unordered_map<glm::ivec3, ChunkData> m_LoadedChunks;
};

