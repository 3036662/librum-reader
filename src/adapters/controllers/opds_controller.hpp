#pragma once
#include "adapters_export.hpp"
#include "opds_model.hpp"
#include "i_opds_controller.hpp"
#include "i_opds_service.hpp"
//#include "opds_model.hpp"


namespace adapters::controllers
{

class ADAPTERS_EXPORT OpdsConrtoller : public IOpdsController
{
    Q_OBJECT
public:
    OpdsConrtoller(application::IOpdsService* opdsService);
    adapters::data_models::OpdsModel*  getOpdsModel() override;
private:
    application::IOpdsService* m_opdsService;
    data_models::OpdsModel m_opdsModel;

};

}  //namespace adapters::controllers
