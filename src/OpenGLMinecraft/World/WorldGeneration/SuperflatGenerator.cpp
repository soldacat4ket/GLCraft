#include "SuperflatGenerator.h"

#include "OpenGLMinecraft/Debug.h"

SuperflatGenerator::SuperflatGenerator(const SuperflatLayout p_Layout)
    :m_WorldLayout(p_Layout)
{
    size_t MaxHeight = 0;
    for(auto Section : m_WorldLayout)
    {
        MaxHeight += Section.Height;
    }
    ASSERT(MaxHeight < CHUNK_SIZE_Y);
}

Chunk* SuperflatGenerator::GenerateChunk(glm::ivec3 p_ChunkPosition)
{
    Chunk* c = new Chunk(p_ChunkPosition);
    c->GenerateCustom(
        [this, &p_ChunkPosition](Chunk::RawChunk& p_Blocks)
        {
            for(size_t x = 0; x < p_Blocks.SizeX(); x++)
            {
                for(size_t z = 0; z < p_Blocks.SizeZ(); z++)
                {
                    size_t TrackedHeight = 0;
                    for(auto Section : m_WorldLayout)
                    {
                        GenerateLayer(Section, p_Blocks, TrackedHeight, x, z);
                    }
                }
            }
        });
    return c;
}

void SuperflatGenerator::GenerateLayer(const SuperflatLayer p_Layer, Chunk::RawChunk& p_Chunk, size_t& p_BlockHeight, const size_t x, const size_t z)
{
    for(size_t FinalHeight = p_BlockHeight; FinalHeight < p_Layer.Height; FinalHeight++)
    {
        p_Chunk(x, FinalHeight, z) = p_Layer.Block;
        p_BlockHeight++;
    }
}
