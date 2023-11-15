#pragma once
#include <QObject>
#include "application_export.hpp"
#include "opds_node.hpp"

namespace application
{

class APPLICATION_EXPORT IOpdsService: public QObject
{
    Q_OBJECT

public:
    virtual ~IOpdsService() noexcept = default;
    virtual const std::vector<domain::value_objects::OpdsNode>&   getOpdsNodes() =0;
    virtual  void loadRootLib(const QString& url)=0;
    virtual void getNodeImage(const QString& id)=0;
public slots:
    virtual void processNodes(const std::vector<domain::value_objects::OpdsNode>&)=0;
signals:
    void nodesVecReplaceStarted();
    void opdsNodesReady();
};

}
