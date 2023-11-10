#pragma once
#include <QObject>
#include "application_export.hpp"

namespace application
{

class APPLICATION_EXPORT IOpdsService: public QObject
{
    Q_OBJECT

public:
    virtual ~IOpdsService() noexcept = default;

};

}
