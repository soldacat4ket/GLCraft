#pragma once

#include <string>
#include <vector>

#include "OpenGLMinecraft/World/WorldGeneration/WorldGenerator.h"

// todo: pregenerate a chunk with the layout and simply copy for generation
// a more customizable superflat world
class SuperflatGenerator : public WorldGenerator
{
public:
    struct SuperflatLayer
    {
        uint16_t Block;
        unsigned int Height;
    };
    using SuperflatLayout = std::vector<SuperflatLayer>;

    SuperflatGenerator(SuperflatLayout p_Layout);

    virtual Chunk::RawChunk GenerateChunk(glm::ivec3 p_ChunkPosition) override;
private:
    Chunk::RawChunk BuildBaseChunk(SuperflatLayout p_Layout);
    void GenerateLayer(uint16_t p_Block, Chunk::RawChunk& p_Chunk, size_t p_BlockHeight);

    Chunk::RawChunk m_BaseChunk;

    SuperflatLayout m_WorldLayout;
};

