#pragma once

#include <vector>

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Pos;
    glm::vec2 Tex;
};

struct Mesh
{
    std::vector<Vertex> Data;
    std::vector<unsigned int> Indices;
};