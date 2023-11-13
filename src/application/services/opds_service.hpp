#pragma once
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include "i_opds_service.hpp"
#include "i_opds_gateway.hpp"

namespace application::services {

class APPLICATION_EXPORT OpdsService : public IOpdsService {
 public:
    OpdsService(IOpdsGateway*  opdsGateway) ;

  const std::vector<domain::value_objects::OpdsNode>& getOpdsNodes() override;
  void loadRootLib(const QString& url) override;

 private:
  IOpdsGateway* m_opdsGateway;
  void loadRootNodesFromFile();
  std::vector<domain::value_objects::OpdsNode> m_opdsNodes;
};

}  // namespace application::services
