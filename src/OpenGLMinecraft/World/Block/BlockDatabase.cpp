#include "BlockDatabase.h"

#include <fstream>

#include "OpenGLMinecraft/Debug.h"
#include "OpenGLMinecraft/Utility/Log.h"
#include "OpenGLMinecraft/Utility/JSONExtensions.h"

BlockDatabase& BlockDatabase::Get()
{
    static BlockDatabase d;
    return d;
}

BlockDatabase::BlockDatabase()
{
    
}

BlockDatabase::~BlockDatabase()
{
    for(auto d : m_Data)
    {
        GUARDED_DELETE(d);
    }
}

BlockData* BlockDatabase::LoadBlockData(nlohmann::json& o)
{
    BlockData* buffer = new BlockData;
    buffer->BlockName = o.at("name").get<std::string>();
    buffer->MeshType = o.at("meshtype").get<std::string>();
    buffer->ShaderType = o.at("shadertype").get<std::string>();
    buffer->IsCollidable = o.at("collidable").get<bool>();
    buffer->IsOpaque = o.at("opaque").get<bool>();

    return buffer;
}

void BlockDatabase::LoadBlocks(const std::filesystem::path& p_BlockBinFile)
{
    std::ifstream BlockBinHandle(p_BlockBinFile);
    ASSERT(BlockBinHandle.is_open());
    
    nlohmann::json TempBlockData = nlohmann::json::parse(BlockBinHandle);

    // todo: add schema validation extension from nlohmann(tha goat)

    std::vector<std::string> TempIdList;
    for(auto& [key, value] : TempBlockData.items())
    {
        LOG_DEBUG("Loading block {} {}", key, value.dump(4));
        TempIdList.push_back(key);
        m_Data.push_back(LoadBlockData(value));
    }

    m_Exchanger.Generate(TempIdList);
}

void BlockDatabase::RegisterReferenceMap(const std::unordered_map<std::string, CachedBlockTextureInfo>& p_TextureMap)
{
    for(auto& [key, value] : p_TextureMap)
    {
        RegisterReference(key, value);
    }
}

void BlockDatabase::RegisterReference(const std::string& p_FullBlockId, const CachedBlockTextureInfo& p_TextureData)
{
    BlockData* Data = m_Data.at(m_Exchanger.Resolve(p_FullBlockId));
    LOG_TRACE("Caching {} texture coords top: {}, bot: {}, side: {}", p_FullBlockId, p_TextureData.TopTexInfo, p_TextureData.BotTexInfo, p_TextureData.SideTexInfo);
    Data->TextureData = p_TextureData;
}

void BlockDatabase::Unload()
{
    m_Data.clear();
}
