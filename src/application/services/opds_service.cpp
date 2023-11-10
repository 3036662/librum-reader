#include "opds_service.hpp"

namespace application::services
{

const std::vector <domain::value_objects::OpdsNode>& OpdsService::getOpdsNodes() {
    m_opdsNodes.emplace_back("Flibusta");
    m_opdsNodes.emplace_back("Coollib");
    m_opdsNodes.emplace_back("Всенаука");
    return m_opdsNodes;
}


  } // namespace application::services
