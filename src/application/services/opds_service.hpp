#pragma once
#include <QDebug>
#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVector>
#include <QStandardPaths>

#include "i_opds_service.hpp"
#include "i_opds_gateway.hpp"

namespace application::services {

using namespace  domain::value_objects;

class APPLICATION_EXPORT OpdsService : public IOpdsService {
 public:
    OpdsService(IOpdsGateway*  opdsGateway) ;

   const std::vector<OpdsNode>& getOpdsNodes() override;
   void loadRootLib(const QString& url) override;
   void getNodeImage(const QString& id) override;
public slots:
   void  processNodes
        (const std::vector<domain::value_objects::OpdsNode>&) override;

 private:
  IOpdsGateway* m_opdsGateway;
  void loadRootNodesFromFile();
  OpdsNode findNodeByUrl(const QString& url) const;

  std::vector<domain::value_objects::OpdsNode> m_opdsNodes;
  std::vector<OpdsNode> historyStack;
  const OpdsNode rootNode{"/","url_root","Root","url_root",""};

};

}  // namespace application::services
