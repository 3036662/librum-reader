#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "i_opds_access.hpp"

namespace infrastructure::persistence{

class OpdsAccess : public adapters::IOpdsAccess
{
    Q_OBJECT
public:
     void loadRootLib(const QString& url) override;
     void getOpdsImage(const QString& id,const QString& url) override;
     void getBookMedia(const QString& id, const QUuid& uuid, const QString& url) override;


private:
     QNetworkAccessManager m_networkAccessManager;
     QNetworkRequest createRequest(const QUrl& url);
};


} // namespace infrastructure::persistence

