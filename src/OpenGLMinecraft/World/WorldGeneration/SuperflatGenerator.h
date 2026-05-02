#pragma once

#include <string>
#include <vector>

#include "OpenGLMinecraft/World/WorldGeneration/WorldGenerator.h"

// todo: pregenerate a chunk with the layout and simply copy for generation
// a more customizable superflat world
class SuperflatGenerator : WorldGenerator
{
public:
    struct SuperflatLayer
    {
        uint16_t Block;
        unsigned int Height;
    };
    using SuperflatLayout = std::vector<SuperflatLayer>;

    SuperflatGenerator(SuperflatLayout p_Layout);

    virtual Chunk* GenerateChunk(glm::ivec3 p_ChunkPosition) override;
private:
    void GenerateLayer(SuperflatLayer p_Layer, Chunk::RawChunk& p_Chunk, size_t& p_BlockHeight, size_t x, size_t z);

    SuperflatLayout m_WorldLayout;
};

