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
                       reply, "Fetching opds feed  page");
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

void OpdsAccess::getOpdsImage(const QString& id,const QString& url){
    auto request = createRequest(url);
    QNetworkReply* reply = m_networkAccessManager.get(request);

    connect(reply,&QNetworkReply::finished, this, [this,id,url,reply](){
         auto success = !api_error_helper::apiRequestFailed(reply, 200);
        if(!success){
            api_error_helper::logErrorMessage(reply,"Getting opds image");
            reply->deleteLater();
            return;
        }
        emit gettingOpdsImageFinished(id,url,reply->readAll());
        reply->deleteLater();
    });


}


} // namespace infrastructure::persistence
