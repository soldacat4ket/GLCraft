#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/Texture.h"
#include "OpenGLMinecraft/MultiMedia/Graphics/Texture/TextureAtlas.h"
#include "OpenGLMinecraft/World/Block/BlockDatabase.h"

//holds opengl texture(for now) and precalculates all block texture coordinates
class TexturePack
{
public:

    // will only load name of texture pack
    TexturePack(const std::filesystem::path& p_PackFile);

    void Load();
    void UploadTexture();

    // return texture data for all blocks in references
    std::unordered_map<std::string, CachedBlockTextureInfo> ConsumeTextureReferences();

    inline const Texture* GetTexture() const { return m_Texture.get(); }
    inline const std::filesystem::path& GetLocation() const { return m_PackLocation; }
    inline const std::string& GetName() const { return m_PackName; }
    inline const std::string& GetDescription() const { return m_PackDescription; }
    inline const std::string& GetAtlasFile() const { return m_AtlasFile; }
    inline const std::string& GetFormat() const { return m_AtlasFormat; }
    inline glm::uvec2 GetGridSize() const { return m_GridSize; }
private:
    void LoadMetadata(const nlohmann::json& p_PackData);
    CachedBlockTextureInfo GenerateTextureCoords(glm::uvec2 p_TopTexPos, glm::uvec2 p_BotTexPos, glm::uvec2 p_SideTexPos, const TextureAtlas& p_Atlas);

    const std::filesystem::path m_PackFile;
    const std::filesystem::path m_PackLocation;
    std::unique_ptr<Texture> m_Texture;
    std::string m_PackName;
    std::string m_PackDescription;
    std::string m_AtlasFile;
    std::string m_AtlasFormat;
    glm::uvec2 m_GridSize;
    nlohmann::json m_References;
};

