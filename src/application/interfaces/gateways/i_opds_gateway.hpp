#pragma once
#include <QObject>

#include "application_export.hpp"


namespace application {

class APPLICATION_EXPORT IOpdsGateway: public QObject{
    Q_OBJECT

public:
    virtual ~IOpdsGateway() noexcept =default;

    virtual void loadRootlib(QString url) = 0;



};


} // namespace application
