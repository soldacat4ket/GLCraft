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

void Config::Load(const std::filesystem::path& p_ConfigPath, const std::filesystem::path& p_BlockDataPath, const std::filesystem::path& p_TexturePath, const std::filesystem::path& p_ShaderPath)
{
    m_ConfigFile = p_ConfigPath;
    m_BlockDataFile = p_BlockDataPath;
    m_TexturePath = p_TexturePath;
    m_ShaderPath = p_ShaderPath;

    LOG_INFO("Config: {}", m_ConfigFile.string());
    LOG_INFO("Block data: {}", m_BlockDataFile.string());
    LOG_INFO("Pack folder: {}", m_TexturePath.string());
    LOG_INFO("Shader folder: {}", m_ShaderPath.string());

    std::ifstream ConfigHandle(m_ConfigFile);
    ASSERT(ConfigHandle.is_open());

    nlohmann::json ConfigData = nlohmann::json::parse(ConfigHandle);

    LoadGraphicsSettings(ConfigData.at("graphicssettings"));
    LoadInputSettings(ConfigData.at("inputsettings"));

    // save to a temporary object since pack loading must be done after the context has be created
    m_TempTextureData = ConfigData.at("texturesettings");
}

void Config::LoadTexturePacks()
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

    m_ActivePackName = m_TempTextureData.at("activepack").get<std::string>();
    LOG_INFO("Searching for pack {}", m_ActivePackName);

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
    m_TempTextureData.clear();
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

void Config::LoadGraphicsSettings(const nlohmann::json& p_GraphicsSettingsData)
{
    m_GraphicsSettings.DesiredWidthPixels = p_GraphicsSettingsData.at("resolution").at(0).get<unsigned int>();
    m_GraphicsSettings.DesiredHeightPixels = p_GraphicsSettingsData.at("resolution").at(1).get<unsigned int>();
    m_GraphicsSettings.Fullscreen = p_GraphicsSettingsData.at("fullscreen").get<bool>();
    m_GraphicsSettings.DesiredFov = p_GraphicsSettingsData.at("fov").get<float>();;
    m_GraphicsSettings.RenderDistanceBlocks = p_GraphicsSettingsData.at("renderdistance").get<float>();;
    m_GraphicsSettings.SolidVertexShaderFile = m_ShaderPath / p_GraphicsSettingsData.at("solidshader").at(0).get<std::filesystem::path>();
    m_GraphicsSettings.SolidFragmentShaderFile = m_ShaderPath / p_GraphicsSettingsData.at("solidshader").at(1).get<std::filesystem::path>();
    m_GraphicsSettings.ChunkLoadDistance = p_GraphicsSettingsData.at("chunkloaddistance").get<unsigned int>();
    m_GraphicsSettings.VSync = p_GraphicsSettingsData.at("vsync").get<bool>();
}

void Config::LoadInputSettings(const nlohmann::json& p_InputSettingsData)
{
    m_InputSettings.UserSensitivity = p_InputSettingsData.at("sensitivity").get<float>();
}
