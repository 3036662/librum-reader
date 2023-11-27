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
                   emit badNetworkResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
                    reply->deleteLater();
                   return;
               }
               QByteArray resp= reply->readAll();
               emit  loadOpdsRootFinished(resp);
               reply->deleteLater();
           });

   connect(reply,&QNetworkReply::redirected,this,[this,reply](const QUrl _url){ this->redirected(reply,_url); });

}


QNetworkRequest OpdsAccess::createRequest(const QUrl& url){
        QNetworkRequest result { url };
        m_networkAccessManager.setRedirectPolicy(QNetworkRequest::UserVerifiedRedirectPolicy);
        //result.setTransferTimeout(3000); // 3 sec timeout
         QSslConfiguration sslConfiguration = result.sslConfiguration();
         sslConfiguration.setProtocol(QSsl::AnyProtocol);
         sslConfiguration.setPeerVerifyMode(QSslSocket::QueryPeer);
         result.setSslConfiguration(sslConfiguration);
         return result;
}

void OpdsAccess::getOpdsImage(const QString& id,const QString& url){
    auto request = createRequest(url);
    QNetworkReply* reply = m_networkAccessManager.get(request);

    connect(reply,&QNetworkReply::redirected,this,[this,reply](const QUrl _url){ this->redirected(reply,_url); });

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

void OpdsAccess::getBookMedia(const QString& id, const QUuid& uuid, const QString& url){
         auto request = createRequest(url);
         auto reply = m_networkAccessManager.get(request);
         connect(reply,&QNetworkReply::redirected,this,[this,reply](const QUrl _url){ this->redirected(reply,_url); });
         connect(reply, &QNetworkReply::readyRead, this,
                 [this, reply, id, uuid]()
                 {
                        QList<QByteArray> headerList = reply->rawHeaderList();
                         foreach(QByteArray head, headerList) {
                            QByteArray  header= reply->rawHeader(head);
                         }
                     if(api_error_helper::apiRequestFailed(reply, 200))
                     {
                         api_error_helper::logErrorMessage(
                             reply, "Getting free book's media");
                        emit badNetworkResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
                         reply->deleteLater();
                         return;
                     }
                     emit gettingBookMediaChunkReady(id, uuid, reply->readAll(),
                                                     "epub", false);
                 });

         connect(reply, &QNetworkReply::finished, this,
                 [this, reply, id, uuid]()
                 {
                     emit gettingBookMediaChunkReady(id, uuid, QByteArray(), "epub",
                                                     true);

                     reply->deleteLater();
                 });

         connect(reply, &QNetworkReply::downloadProgress, this,
                 [this, id](qint64 bytesReceived, qint64 bytesTotal)
                 {
                     emit gettingBookMediaProgressChanged(id, bytesReceived,
                                                          bytesTotal);
                 });

         connect (reply, &QNetworkReply::errorOccurred, this, [this](QNetworkReply::NetworkError code){
             qDebug() << code;
         });

}

void OpdsAccess::redirected( QNetworkReply* const reply,const QUrl& url){
    // alow all redirects
    qDebug() << url.toString();
    emit reply->redirectAllowed();
}


} // namespace infrastructure::persistence
