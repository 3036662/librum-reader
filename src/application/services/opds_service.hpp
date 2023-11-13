#pragma once
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

#include "i_opds_service.hpp"

namespace application::services {

class APPLICATION_EXPORT OpdsService : public IOpdsService {
 public:
  const std::vector<domain::value_objects::OpdsNode>& getOpdsNodes() override;

 private:
  void loadRootNodesFromFile();
  std::vector<domain::value_objects::OpdsNode> m_opdsNodes;
};

}  // namespace application::services
