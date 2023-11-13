#pragma once
#include "i_opds_access.hpp"

namespace infrastructure::persistence{

class OpdsAccess : public adapters::IOpdsAccess
{
    Q_OBJECT
public:
     void loadRootLib(const QString& url);
};


} // namespace infrastructure::persistence

