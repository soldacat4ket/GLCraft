#include "TexturePack.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/Utility/JSONExtensions.h"

TexturePack::TexturePack(const std::filesystem::path& p_PackFile)
    :m_Texture(nullptr), m_References(nullptr), m_PackFile(p_PackFile), m_PackLocation(p_PackFile.parent_path()), m_GridSize({0,0})
{
    std::ifstream PackHandle(m_PackFile);
    ASSERT(PackHandle.is_open());

    // load pack name only for searching purposes before full loading
    m_PackName = nlohmann::json::parse(PackHandle).at("packname").get<std::string>();
}

void TexturePack::Load()
{
    std::ifstream PackHandle(m_PackFile);
    ASSERT(PackHandle.is_open());

    // load block info
    nlohmann::json PackInfo = nlohmann::json::parse(PackHandle);
    m_References = PackInfo.at("references"); // store direct json references object for later processing by RegisterAllReferences()

    LoadMetadata(PackInfo);
}

void TexturePack::UploadTexture()
{
    m_Texture = std::make_unique<Texture>(m_PackLocation / m_AtlasFile);
}

std::unordered_map<std::string, CachedBlockTextureInfo> TexturePack::ConsumeTextureReferences()
{
    ASSERT(m_Texture);
    bool CanGridFitReferences = (m_GridSize.x * m_GridSize.y) >= m_References.size();
    ASSERT(CanGridFitReferences);

    TextureAtlas Atlas({m_Texture->GetWidth(), m_Texture->GetHeight()}, m_GridSize);
    std::unordered_map<std::string, CachedBlockTextureInfo> buffer;

    auto StartTime = std::chrono::steady_clock::now();

    for(auto& [key, value] : m_References.items())
    {
        glm::uvec2 Top, Bot, Side;

        if(value.contains("texall"))
        {
            Top = value.at("texall").get<glm::uvec2>();
            Bot = value.at("texall").get<glm::uvec2>();
            Side = value.at("texall").get<glm::uvec2>();
        }
        else
        {
            Top = value.at("textop").get<glm::uvec2>();
            Bot = value.at("texbot").get<glm::uvec2>();
            Side = value.at("texside").get<glm::uvec2>();
        }
        buffer.emplace(key, GenerateTextureCoords(Top, Bot, Side, Atlas));
    }

    auto EndTime = std::chrono::steady_clock::now();
    auto TimeToComplete = std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - StartTime);

    LOG_DEBUG("Generated texture references for pack {} in {}ms", m_PackName, TimeToComplete.count());

    m_References.clear();

    return buffer;
}

void TexturePack::LoadMetadata(const nlohmann::json& p_PackData)
{
    m_PackDescription = p_PackData.at("packdescription").get<std::string>();
    m_AtlasFormat = p_PackData.at("atlasformat").get<std::string>();
    m_AtlasFile = p_PackData.at("atlasfile").get<std::string>() + "." + m_AtlasFormat;
    m_GridSize = p_PackData.at("gridsize").get<glm::uvec2>();
}

CachedBlockTextureInfo TexturePack::GenerateTextureCoords(glm::uvec2 p_TopTexPos, glm::uvec2 p_BotTexPos, glm::uvec2 p_SideTexPos, const TextureAtlas& p_Atlas)
{
    CachedBlockTextureInfo buffer;
    buffer.TopTexInfo = p_Atlas.GetTexture2d(p_TopTexPos.x, p_TopTexPos.y);
    buffer.BotTexInfo = p_Atlas.GetTexture2d(p_BotTexPos.x, p_BotTexPos.y);
    buffer.SideTexInfo = p_Atlas.GetTexture2d(p_SideTexPos.x, p_SideTexPos.y);
    return buffer;
}
