#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stdint.h>

#include "OpenGLMinecraft/Utility/Singleton.h"

class BlockIdExchanger
{
public:
    BlockIdExchanger();
    BlockIdExchanger(const std::vector<std::string>& p_Ids);

    void Generate(const std::vector<std::string>& p_Ids);

    inline uint16_t Resolve(const std::string& p_Id) const { return m_FastStringMap.at(p_Id); };
    inline const std::string& Resolve(uint16_t p_Id) const { return m_MasterIdArray.at(p_Id); };
private:

    std::vector<std::string> m_MasterIdArray;
    std::unordered_map<std::string, uint16_t> m_FastStringMap;
};

