#pragma once
#include "adapters_export.hpp"
#include "i_opds_gateway.hpp"
#include "i_opds_access.hpp"

namespace  adapters::gateways {


class ADAPTERS_EXPORT OpdsGateway
    : public application::IOpdsGateway
{
    Q_OBJECT

public:
    OpdsGateway(IOpdsAccess* opdsAccess);

    void loadRootlib(QString url) override;

private:
    IOpdsAccess* m_OpdsAccess;
};

} // namespace adapters::gateways
