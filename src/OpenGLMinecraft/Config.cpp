#include "Config.h"

#include <fstream>

#include <nlohmann/json.hpp>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"

namespace fs = std::filesystem;

Config& Config::Get()
{
    static Config c;
    return c;
}

void Config::Load(const std::filesystem::path& p_ConfigPath, const std::filesystem::path& p_BlockDataPath, const std::filesystem::path& p_TexturePath)
{
    m_ConfigPath = p_ConfigPath;
    m_BlockDataPath = p_BlockDataPath;
    m_TexturePath = p_TexturePath;

    LOG_INFO("Config: {}", m_ConfigPath.string());
    LOG_INFO("Block data: {}", m_BlockDataPath.string());
    LOG_INFO("Pack folder: {}", m_TexturePath.string());

    std::ifstream ConfigHandle(m_ConfigPath);
    ASSERT(ConfigHandle.is_open());

    nlohmann::json ConfigData = nlohmann::json::parse(ConfigHandle);

    LoadInputSettings(ConfigData.at("inputsettings"));
    LoadTexturePacks(ConfigData.at("texturesettings"));
}


std::unordered_map<std::string, CachedBlockTextureInfo> Config::BuildTextureReferences()
{
    // todo: potentially add stacking logic for a minecraft-like list of active texture packs
    // for now just upload the active packs image to the gpu
    m_ActivePack->UploadTexture();
    // and return the map of texture coordinates to block ids
    return m_ActivePack->ConsumeTextureReferences();
}

Config::Config()
    :m_ActivePack(nullptr), m_InputSettings({1.0f})
{

}

void Config::LoadInputSettings(const nlohmann::json& p_InputSettingsData)
{
    m_InputSettings.UserSensitivity = p_InputSettingsData.at("sensitivity").get<float>();
}

void Config::LoadTexturePacks(const nlohmann::json& p_TextureConfigData)
{
    // scan for all json files in packs folder
    std::vector<std::filesystem::path> FoundPacks;
    for(const auto& entry: fs::recursive_directory_iterator(m_TexturePath))
    {
        if(entry.is_regular_file() && entry.path().extension() == ".json")
        {
            LOG_DEBUG("Found pack {}", entry.path().string());
            FoundPacks.push_back(entry.path());
        }
    }

    m_ActivePackName = p_TextureConfigData.at("activepack").get<std::string>();
    LOG_DEBUG("Searching for pack {}", m_ActivePackName);

    // search for active pack by name and load it
    bool HasFoundPack = false;
    for(auto pack: FoundPacks)
    {
        auto NameOnlyPack = TexturePack(pack);
        if(NameOnlyPack.GetName() == m_ActivePackName)
        {
            LOG_DEBUG("Found match at {}", pack.string());

            m_ActivePack = std::make_unique<TexturePack>(pack);
            m_ActivePack->Load();
            HasFoundPack = true;
            break;
        }
    }

    ASSERT(HasFoundPack);
}
