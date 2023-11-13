#include "opds_gateway.hpp"

namespace adapters::gateways{

OpdsGateway::OpdsGateway(IOpdsAccess* opdsAccess)
    : m_OpdsAccess(opdsAccess){

}

void OpdsGateway::loadRootlib(QString url){
    m_OpdsAccess->loadRootLib(url);
}

} // namespace adapters::gateways
