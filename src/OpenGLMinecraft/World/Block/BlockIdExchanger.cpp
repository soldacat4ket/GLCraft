#include "BlockIdExchanger.h"

#include "OpenGLMinecraft/Utility/Log.h"

BlockIdExchanger::BlockIdExchanger()
{
}

BlockIdExchanger::BlockIdExchanger(const std::vector<std::string>& p_Ids)
{
    Generate(p_Ids);
}

void BlockIdExchanger::Generate(const std::vector<std::string>& p_Ids)
{
    LOG_INFO("Generating BlockID map");
    m_MasterIdArray.clear();
    m_FastStringMap.clear();
    for(int i = 0; i < p_Ids.size(); i++)
    {
        LOG_DEBUG("New ID Entry: {} | {}", i, p_Ids[i]);
        m_MasterIdArray.push_back(p_Ids[i]);
        m_FastStringMap.emplace(p_Ids[i], i);
    }
}
