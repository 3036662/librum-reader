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

     // allow all redirects even to plain http
     inline void redirected(QNetworkReply* const reply,const QUrl& url);
      // change format of file depending on headers of response
     inline QString changeFormat(const QNetworkReply*   const reply,const QString& format) const;
};


} // namespace infrastructure::persistence

