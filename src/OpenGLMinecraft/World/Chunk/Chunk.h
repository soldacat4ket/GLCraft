#pragma once

#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include "OpenGLMinecraft/Utility/Array3D.h"

// fixed height chunks of 256 blocks, could update to 16 for an infinate build height
#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 256
#define CHUNK_SIZE_Z 16
#define CHUNK_SIZE_TOTAL CHUNK_SIZE_X*CHUNK_SIZE_Y*CHUNK_SIZE_Z

// https://en.wikipedia.org/wiki/Run-length_encoding
struct ChunkRLESection
{
    unsigned int Count;
    uint16_t Id;
};

// remove position attribute from chunk? member is used for mesh generation based on position but World holds position as keys to corresponding chunks
class Chunk
{
public:
    using RLECompressedChunk = std::vector<ChunkRLESection>;
    using RawChunk = Array3D<uint16_t>;

    // find use since chunks are no longer stored compressed, maybe for world save data
    static RawChunk DecompressRLE(const RLECompressedChunk& p_Data);
    static RLECompressedChunk CompressRLE(const RawChunk& p_Data);

    Chunk(glm::ivec3 p_ChunkPosition);

    void Clear();

    // fills chunk, bedrock -> 3 stone -> dirt -> grass
    void Superflat();

    void GenerateCustom(std::function<void(RawChunk&)> p_GenerationFunction);
    void EditBlock(glm::uvec3 p_BlockPos, uint16_t p_NewBlock);
    inline void WriteBlocks(RawChunk c) { m_Data = c; }

    inline glm::ivec3 GetPos() const { return m_Position; }
    inline const RawChunk& GetBlocks() const { return m_Data; }
private:

    RawChunk m_Data;
    glm::ivec3 m_Position;
};

