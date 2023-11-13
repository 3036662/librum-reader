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
    virtual  void loadRootLib(QString url)=0;
};

}
