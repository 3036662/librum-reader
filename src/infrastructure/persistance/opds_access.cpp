#include "opds_access.hpp"
#include "api_error_helper.hpp"

namespace infrastructure::persistence{

void OpdsAccess::loadRootLib(const QString& url){
   QNetworkRequest request = createRequest(url);
   QNetworkReply* reply = m_networkAccessManager.get(request);

    connect (reply, &QNetworkReply::finished,this,
           [this,reply](){
                    auto success = !api_error_helper::apiRequestFailed(reply, 200);
               if(!success){
                   api_error_helper::logErrorMessage(
                       reply, "Fetching first free books metadata page");
                    reply->deleteLater();
                   return;
               }
               QByteArray resp= reply->readAll();
               emit  loadOpdsRootFinished(resp);
               reply->deleteLater();
           });
}


QNetworkRequest OpdsAccess::createRequest(const QUrl& url){
        QNetworkRequest result { url };
         result.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
         QSslConfiguration sslConfiguration = result.sslConfiguration();
         sslConfiguration.setProtocol(QSsl::AnyProtocol);
         sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
         result.setSslConfiguration(sslConfiguration);
         return result;
}


} // namespace infrastructure::persistence
