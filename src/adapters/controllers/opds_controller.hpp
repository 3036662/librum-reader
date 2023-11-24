#pragma once
#include "adapters_export.hpp"
#include "opds_model.hpp"
#include "i_opds_controller.hpp"
#include "i_opds_service.hpp"


namespace adapters::controllers
{

class ADAPTERS_EXPORT OpdsConrtoller : public IOpdsController
{
    Q_OBJECT
public:
    OpdsConrtoller(application::IOpdsService* opdsService);
    adapters::data_models::OpdsModel*  getOpdsModel() override;
    void loadRootLib(const QString& url) override;
    void getNodeImage(const QString& id)  override;
    void handleBadNetworkResponse(int) override;
    void getBookMedia(const QString& id, const QString& downloadUrl) override;
    void markBookAsDownloaded(const QString& id) override;
    //void deleteNodeImage(const QString &id) override;
private:
    application::IOpdsService* m_opdsService; // pointer to service
    data_models::OpdsModel m_opdsModel; // object Model

};

}  //namespace adapters::controllers
