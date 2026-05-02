#pragma once

#include <glm/glm.hpp>

#include "OpenGLMinecraft/World/Chunk/Chunk.h"

class WorldGenerator
{
public:
    // WILL ALLOCATE ON THE HEAP, DELETE THEM
    virtual Chunk* GenerateChunk(glm::ivec3 p_ChunkPosition) = 0;
private:
};