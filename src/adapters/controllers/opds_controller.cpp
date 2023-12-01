#include "opds_controller.hpp"
#include <QUrl>

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

    // getting book media
    connect(m_opdsService,
            &application::IOpdsService::gettingBookFinished, this,
            &OpdsConrtoller::gettingBookFinished);

    connect(m_opdsService,
            &application::IOpdsService::downloadingBookMediaProgressChanged,
            &m_opdsModel,
            &data_models::OpdsModel::downloadingBookMediaProgressChanged
            );

    connect(m_opdsService,
            &application::IOpdsService::bookIsDownloadedChanged,
            &m_opdsModel,
            &data_models::OpdsModel::bookIsDownloadedChanged);

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

void OpdsConrtoller::getBookMedia(const QString& id, const QString& downloadUrl) {
    m_opdsService->getBookMedia(id,downloadUrl);
}

void OpdsConrtoller::markBookAsDownloaded(const QString& id) {
    m_opdsService->markBookAsDownloaded(id);
}

void OpdsConrtoller::saveOpdsLib(const QString& title, const QString& url, const QString descr) {
    QUrl tempUrl(url);
    if (title.isEmpty() || !tempUrl.isValid() || tempUrl.host().isEmpty() || tempUrl.scheme().isEmpty() ){
        emit badOpdsLibData("Please enter valid title and url");
        return;
    }
    if (m_opdsService->saveOpdsLib(title,url,descr)){
        emit savingLibFinished();
        m_opdsService->loadRootLib("url_root");
    }
}

} //namespace  adapters::controllers
