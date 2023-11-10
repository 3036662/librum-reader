#pragma once
#include "adapters_export.hpp"
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


};

}  //namespace adapters::controllers
