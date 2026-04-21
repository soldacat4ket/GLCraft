#include "TextureAtlas.h"

#include <cmath>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

TextureAtlas::TextureAtlas(const glm::uvec2 p_ImageResolutionPixels, const glm::uvec2 p_GridSize)
    :m_ImageSizePixels(p_ImageResolutionPixels), m_GridSize(p_GridSize)
{
    bool DoesAtlasSizeConformToGridSize = std::fmod(m_ImageSizePixels.x, p_GridSize.x) == 0.0f && fmod(m_ImageSizePixels.x, p_GridSize.y) == 0.0f;
    ASSERT(DoesAtlasSizeConformToGridSize);

    m_SingleTextureResolutionPixels = m_ImageSizePixels / m_GridSize;
    m_PixelSizeNormalized = glm::vec2(1.0f / m_ImageSizePixels.x, 1.0f / m_ImageSizePixels.y);
    m_TextureSizeNormalized = glm::vec2(m_SingleTextureResolutionPixels) * m_PixelSizeNormalized;
}

std::array<float, 8> TextureAtlas::GetTexture2d(const unsigned int& p_X, const unsigned int& p_Y) const
{
    bool IsTextureInBounds = p_X < m_GridSize.x || p_X >= 0 ||
        p_Y < m_GridSize.y || p_Y >= 0;
    ASSERT(IsTextureInBounds);

    LOG_TRACE("Resolving texture [{}, {}] in grid [{}, {}]", p_X, p_Y, m_GridSize.x, m_GridSize.y);

    const float xMin = (p_X * m_TextureSizeNormalized.x);
    const float yMin = (p_Y * m_TextureSizeNormalized.y);
    const float xMax = (xMin + m_TextureSizeNormalized.x);
    const float yMax = (yMin + m_TextureSizeNormalized.y);

    const std::array<float, 8> Result = {
        xMax, yMax, // bottom left
        xMin, yMax, // bottom right
        xMin, yMin, // top right
        xMax, yMin // top left
    };

    LOG_TRACE("Resolved to {}", Result);
    return Result;
}