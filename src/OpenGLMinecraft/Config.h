#pragma once

#include <memory>
#include <filesystem>

#include "OpenGLMinecraft/TexturePack.h"
#include "OpenGLMinecraft/Utility/Singleton.h"

#define DEFAULT_CONFIG_FILE std::filesystem::current_path() / "res" / "settings.json"
#define DEFAULT_BLOCKDATA_FILE std::filesystem::current_path() / "res" / "blockdata.json"
#define DEFAULT_SHADER_PATH std::filesystem::current_path() / "res" / "shaders"
#define DEFAULT_TEXTURE_PATH std::filesystem::current_path() / "res" / "textures"

class Config : Singleton
{
public:
    static Config& Get();

    void Load(const std::filesystem::path& p_ConfigPath = DEFAULT_CONFIG_FILE, 
              const std::filesystem::path& p_BlockDataPath = DEFAULT_BLOCKDATA_FILE, 
              const std::filesystem::path& p_TexturePath = DEFAULT_TEXTURE_PATH,
              const std::filesystem::path& p_ShaderPath = DEFAULT_SHADER_PATH);

    void LoadTexturePacks();

    std::unordered_map<std::string, CachedBlockTextureInfo> BuildTextureReferences();

    inline const TexturePack* GetActivePack() const { return m_ActivePack.get(); }
    inline const std::string& GetActivePackName() const { return m_ActivePackName; }

    inline const std::filesystem::path& GetTexturePath() const { return m_TexturePath; }
    inline const std::filesystem::path& GetBlockDataFile() const { return m_BlockDataFile; }

    struct GraphicsSettings
    {
        unsigned int DesiredWidthPixels;
        unsigned int DesiredHeightPixels;
        bool Fullscreen;
        std::filesystem::path SolidVertexShaderFile;
        std::filesystem::path SolidFragmentShaderFile;
    };
    inline const GraphicsSettings& GetGraphicsSettings() const { return m_GraphicsSettings; }

    struct InputSettings
    {
        float UserSensitivity;
    };
    inline const InputSettings& GetInputSettings() const { return m_InputSettings; }

private:
    Config();
    
    void LoadGraphicsSettings(const nlohmann::json& p_GraphicsSettingsData);
    void LoadInputSettings(const nlohmann::json& p_InputSettingsData);

    nlohmann::json m_TempTextureData;

    std::unique_ptr<TexturePack> m_ActivePack;

    GraphicsSettings m_GraphicsSettings;
    InputSettings m_InputSettings;

    std::filesystem::path m_TexturePath;
    std::filesystem::path m_ShaderPath;
    std::filesystem::path m_BlockDataFile;
    std::filesystem::path m_ConfigFile;
    std::string m_ActivePackName;
};

