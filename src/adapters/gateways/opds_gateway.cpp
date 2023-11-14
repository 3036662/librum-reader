#include "opds_gateway.hpp"

namespace adapters::gateways{

OpdsGateway::OpdsGateway(IOpdsAccess* opdsAccess)
    : m_OpdsAccess(opdsAccess){
    connect(m_OpdsAccess,
        &IOpdsAccess::loadOpdsRootFinished,this,
            &OpdsGateway::parseOpdsResonse);

}

void OpdsGateway::loadRootlib(QString url){
    m_OpdsAccess->loadRootLib(url);
}

void OpdsGateway::parseOpdsResonse(const QByteArray& data){

}

} // namespace adapters::gateways
