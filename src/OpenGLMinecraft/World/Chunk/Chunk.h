#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/Utility/Array3D.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16
#define CHUNK_SIZE_TOTAL CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z

// https://en.wikipedia.org/wiki/Run-length_encoding
struct ChunkRLESection
{
    unsigned int Count;
    uint16_t Id;
};

class Chunk
{
public:
    using RLECompressedChunk = std::vector<ChunkRLESection>;
    using RawChunk = Array3D<uint16_t>;

    // find use since chunks are no longer stored compressed, maybe for world save data
    static RawChunk DecompressRLE(const RLECompressedChunk& p_Data);
    static RLECompressedChunk CompressRLE(const RawChunk& p_Data);

    Chunk(glm::ivec3 p_ChunkPosition);

    void GenerateCustom(std::function<void(RawChunk&)> p_GenerationFunction);

    // generate the chunk such that the bottom layer is bedrock, the next 3 are stone, then 1 layer of dirt, then 1 layer of grass, with the rest filled as air
    void Superflat();

    inline glm::ivec3 GetPos() const { return m_Position; }
    inline const RawChunk& GetBlocks() const { return m_Data; }
private:

    RawChunk m_Data;
    glm::ivec3 m_Position;
};

