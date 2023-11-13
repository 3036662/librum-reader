#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "i_opds_access.hpp"

namespace infrastructure::persistence{

class OpdsAccess : public adapters::IOpdsAccess
{
    Q_OBJECT
public:
     void loadRootLib(const QString& url);

private:
     QNetworkAccessManager m_networkAccessManager;

     QNetworkRequest createRequest(const QUrl& url);
};


} // namespace infrastructure::persistence

