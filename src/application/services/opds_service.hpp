#pragma once
#include "i_opds_service.hpp"

namespace application::services
{


class APPLICATION_EXPORT OpdsService : public IOpdsService
{
public:
    const std::vector <domain::value_objects::OpdsNode>& getOpdsNodes() override;

private:
    std::vector <domain::value_objects::OpdsNode> m_opdsNodes;
};

} // namespace application::services
