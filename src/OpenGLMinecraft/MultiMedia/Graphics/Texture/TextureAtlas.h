#pragma once

#include <array>

#include <glm/glm.hpp>

// https://0fps.net/2013/07/09/texture-atlases-wrapping-and-mip-mapping/
// takes image dimensions and a grid size and maps grid coordinates to their respective opengl texture coordinates
class TextureAtlas
{
public:
    TextureAtlas(glm::uvec2 p_ImageResolutionPixels, glm::uvec2 p_GridSize);

    std::array<float, 8> GetTexture2d(const unsigned int& p_X, const unsigned int& p_Y) const;

private:
    glm::uvec2 m_ImageSizePixels;
    glm::uvec2 m_GridSize;
    glm::uvec2 m_SingleTextureResolutionPixels;
    glm::vec2 m_PixelSizeNormalized;
    glm::vec2 m_TextureSizeNormalized;
};