#pragma once

#include <memory>
#include <vector>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include "OpenGLMinecraft/Utility/Singleton.h"
#include "OpenGLMinecraft/World/Block/BlockIdExchanger.h"

/*
blockbin json layout
    "blocksource:blockid"{
        "name": string // name of block
        "opaque": bool // invisible or transparent
        "collidable": bool // collidable or not
        "meshtype": "block" | "flora" // what kind of mesh does it have
        "shadertype": "block" | "liquid" | "flora" // what kind of shader renders this block
    }
*/

namespace GlobalBlockData
{
    struct AdjacentBlocks
    {
        inline void Of(const int& x, const int& y, const int& z)
        {
            Up = {x, y + 1, z};
            Down = {x, y - 1, z};
            Left = {x - 1, y, z};
            Right = {x + 1, y, z};
            Front = {x, y, z + 1};
            Back = {x, y, z - 1};
        }

        glm::ivec3 Up, Down, Left, Right, Front, Back;
    };

    // https://www.desmos.com/3d really good for visualizing faces
    // vertices are defined in ccw order

    const std::array<float, 12> FrontFace = {
        0.0f, 0.0f, 1.0f, // bottom left
        1.0f, 0.0f, 1.0f, // bottom right
        1.0f, 1.0f, 1.0f, // top right
        0.0f, 1.0f, 1.0f  // top left
    };

    const std::array<float, 12> BackFace = {
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f
    };

    const std::array<float, 12> LeftFace = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };

    const std::array<float, 12> RightFace = {
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,
    };

    const std::array<float, 12> TopFace = {
        0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    const std::array<float, 12> BottomFace = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    const std::array<float, 12> XFace1 = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 0.0f
    };

    const std::array<float, 12> xFace2 = {
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f
    };
}

// texture coords, for a block, should maybe add some kind of all and x face arrays w/ options to reduce redundant data
struct CachedBlockTextureInfo
{
    std::array<float, 8> TopTexInfo;
    std::array<float, 8> BotTexInfo;
    std::array<float, 8> SideTexInfo;
};

struct BlockData
{
    std::string BlockName;
    CachedBlockTextureInfo TextureData;
    std::string MeshType;
    std::string ShaderType;

    bool IsOpaque;
    bool IsCollidable;
};

class BlockDatabase: Singleton
{
public:
    static BlockDatabase& Get();

    // load all blockdata
    void LoadBlocks(const std::filesystem::path& p_BlockBinFile);

    // add texture info to block ids
    void RegisterReferenceMap(const std::unordered_map<std::string, CachedBlockTextureInfo>& p_TextureMap);
    void RegisterReference(const std::string& p_FullBlockId, const CachedBlockTextureInfo& p_TextureData);

    void Unload();

    inline const BlockIdExchanger& Exchanger() const { return m_Exchanger; }

    inline const BlockData& GetData(uint16_t p_Id) const { return *m_Data.at(p_Id); }

private:
    BlockDatabase();
    ~BlockDatabase();
    // load a single blocks data object
    BlockData* LoadBlockData(nlohmann::json& p_DataObject);

    BlockIdExchanger m_Exchanger;
    std::vector<BlockData*> m_Data;
};

