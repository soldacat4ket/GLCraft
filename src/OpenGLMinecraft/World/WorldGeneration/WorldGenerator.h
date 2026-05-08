#pragma once

#include <glm/glm.hpp>

#include "OpenGLMinecraft/World/Chunk/Chunk.h"

class WorldGenerator
{
public:
    virtual Chunk::RawChunk GenerateChunk(glm::ivec3 p_ChunkPosition) = 0;
private:
};