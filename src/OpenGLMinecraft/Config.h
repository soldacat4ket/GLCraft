#pragma once

#include <memory>
#include <filesystem>

#include "OpenGLMinecraft/TexturePack.h"
#include "OpenGLMinecraft/Utility/Singleton.h"

#define DEFAULT_CONFIG_FILE std::filesystem::current_path() / "res" / "settings.json"
#define DEFAULT_BLOCKDATA_FILE std::filesystem::current_path() / "res" / "blockdata.json"
#define DEFAULT_TEXTURE_PATH std::filesystem::current_path() / "res" / "textures"

class Config : Singleton
{
public:
    static Config& Get();

    void Load(const std::filesystem::path& p_ConfigPath = DEFAULT_CONFIG_FILE, 
              const std::filesystem::path& p_BlockDataPath = DEFAULT_BLOCKDATA_FILE, 
              const std::filesystem::path& p_TexturePath = DEFAULT_TEXTURE_PATH);

    std::unordered_map<std::string, CachedBlockTextureInfo> BuildTextureReferences();

    inline const TexturePack* GetActivePack() const { return m_ActivePack.get(); }
    inline const std::filesystem::path& GetTexturePath() const { return m_TexturePath; }
    inline const std::filesystem::path& GetBlockDataPath() const { return m_BlockDataPath; }
    inline const std::string& GetActivePackName() const { return m_ActivePackName; }

    struct InputSettings
    {
        float UserSensitivity;
    };
    inline const InputSettings& GetInputSettings() const { return m_InputSettings; }

private:
    Config();

    void LoadInputSettings(const nlohmann::json& p_InputSettingsData);
    void LoadTexturePacks(const nlohmann::json& p_TextureConfigData);

    std::unique_ptr<TexturePack> m_ActivePack;
    InputSettings m_InputSettings;
    std::filesystem::path m_TexturePath;
    std::filesystem::path m_BlockDataPath;
    std::filesystem::path m_ConfigPath;
    std::string m_ActivePackName;
};

