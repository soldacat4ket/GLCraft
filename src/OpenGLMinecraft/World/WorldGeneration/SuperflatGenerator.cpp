#include "SuperflatGenerator.h"

#include "OpenGLMinecraft/Debug.h"

SuperflatGenerator::SuperflatGenerator(const SuperflatLayout p_Layout)
    :m_WorldLayout(p_Layout), m_BaseChunk(BuildBaseChunk(p_Layout))
{
    size_t MaxHeight = 0;
    for(auto Section : m_WorldLayout)
    {
        MaxHeight += Section.Height;
    }
    ASSERT(MaxHeight < CHUNK_SIZE_Y);
}

Chunk::RawChunk SuperflatGenerator::GenerateChunk(glm::ivec3 p_ChunkPosition)
{
    return m_BaseChunk;
}

Chunk::RawChunk SuperflatGenerator::BuildBaseChunk(const SuperflatLayout p_Layout)
{
    Chunk c = Chunk({0,0,0});
    c.GenerateCustom(
        [this, p_Layout](Chunk::RawChunk& p_Blocks)
        {
            size_t TrackedHeight = 0;
            for(auto Section : p_Layout)
            {
                for(size_t y = TrackedHeight; y < TrackedHeight + Section.Height; y++)
                {
                    GenerateLayer(Section.Block, p_Blocks, y);
                }
                TrackedHeight += Section.Height;
            }
        });
    return c.GetBlocks();
}

void SuperflatGenerator::GenerateLayer(const uint16_t p_Block, Chunk::RawChunk& p_Chunk, const size_t p_BlockHeight)
{
    for(size_t x = 0; x < p_Chunk.SizeX(); x++)
    {
        for(size_t z = 0; z < p_Chunk.SizeZ(); z++)
        {
            p_Chunk(x, p_BlockHeight, z) = p_Block;
        }
    }          
}
