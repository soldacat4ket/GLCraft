#include "Chunk.h"

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/World/Block/BlockDatabase.h"

Chunk::RawChunk Chunk::DecompressRLE(const RLECompressedChunk& p_Data)
{
    RawChunk buffer = RawChunk(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z);
    int index = 0;
    for(const auto& RLESegment : p_Data)
    {
        for(unsigned int i = 0; i < RLESegment.Count; i++)
        {
            buffer.Data()[index] = RLESegment.Id;
            index++;
        }
    }

    return buffer;
}

Chunk::RLECompressedChunk Chunk::CompressRLE(const RawChunk& p_Data)
{
    RLECompressedChunk buffer;
    uint16_t currentlyTrackedId = p_Data.Data()[0];
    unsigned int currentCount = 1;
    for(int i = 1; i < CHUNK_SIZE_TOTAL; i++)
    {
        if(p_Data.Data()[i] == currentlyTrackedId)
        {
            currentCount++;
        }
        else
        {
            buffer.push_back({currentCount, currentlyTrackedId});
            currentlyTrackedId = p_Data.Data()[i];
            currentCount = 1;
        }
    }

    LOG_DEBUG("Compressed {} of chunk data to {}", sizeof(p_Data), sizeof(buffer));

    return buffer;
}

Chunk::Chunk(const glm::ivec3 p_ChunkPosition)
    :m_Position(p_ChunkPosition), m_Data(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z)
{
    Clear();
}

void Chunk::Clear()
{
    m_Data.Fill(BlockDatabase::Get().Exchanger().Resolve("vanilla:air"));
}

void Chunk::Superflat()
{
    for(size_t z = 0; z < CHUNK_SIZE_Z; z++)
    {
        for(size_t x = 0; x < CHUNK_SIZE_X; x++)
        {
            m_Data(x, 0, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:bedrock_block");
            m_Data(x, 1, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:stone_block");
            m_Data(x, 2, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:stone_block");
            m_Data(x, 3, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:stone_block");
            m_Data(x, 4, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:dirt_block");
            m_Data(x, 5, z) = BlockDatabase::Get().Exchanger().Resolve("vanilla:grass_block");
        }
    }
}

void Chunk::GenerateCustom(std::function<void(RawChunk&)> p_GenerationFunction)
{
    if(p_GenerationFunction) p_GenerationFunction(m_Data);
}