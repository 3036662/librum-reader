#include "opds_controller.hpp"

namespace  adapters::controllers {

OpdsConrtoller::OpdsConrtoller(
    application::IOpdsService* opdsService):
    m_opdsService(opdsService),
    m_opdsModel(&m_opdsService->getOpdsNodes())
{
    connect(
        m_opdsService,&application::IOpdsService::nodesVecReplaceStarted,
        &m_opdsModel,&adapters::data_models::OpdsModel::startedDataChange
        );

    connect(
        m_opdsService,&application::IOpdsService::opdsNodesReady,
        &m_opdsModel,&adapters::data_models::OpdsModel::completedDataChange
        );
    connect(m_opdsService, &application::IOpdsService::dataChanged,
            &m_opdsModel, &data_models::OpdsModel::refreshNode);

    connect(m_opdsService,&application::IOpdsService::badNetworkResponse,
            this,&IOpdsController::handleBadNetworkResponse);

}

adapters::data_models::OpdsModel*  OpdsConrtoller::getOpdsModel(){
    return &m_opdsModel;
}

void OpdsConrtoller::loadRootLib(const QString& url){
    if (url.isEmpty())
        return;
    m_opdsService->loadRootLib(url);
}

void OpdsConrtoller::getNodeImage(const QString& id) {
    if (id.isEmpty()) return;
    m_opdsService->getNodeImage(id);
}

//void OpdsConrtoller::deleteNodeImage(const QString &id){
//    if (id.isEmpty()) return;
//    m_opdsService->deleteNodeImage(id);
//}

void OpdsConrtoller::handleBadNetworkResponse(int code){
    m_opdsModel.startedDataChange();
    m_opdsModel.completedDataChange();
    emit badNetworkResponse(code);
}


} //namespace  adapters::controllers
