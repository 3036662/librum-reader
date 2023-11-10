#include "opds_controller.hpp"

namespace  adapters::controllers {

OpdsConrtoller::OpdsConrtoller(
    application::IOpdsService* opdsService):
    m_opdsService(opdsService),
    m_opdsModel(&m_opdsService->getOpdsNodes())
{

}

adapters::data_models::OpdsModel*  OpdsConrtoller::getOpdsModel(){
    return &m_opdsModel;
}

} //namespace  adapters::controllers
