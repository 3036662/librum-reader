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
        result.setTransferTimeout(5000); // 5 sec timeout
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

void OpdsAccess::getBookMedia(const QString& id, const QUuid& uuid, const QString& _url){
    qsizetype format_pos=_url.lastIndexOf("_");
    // if not downloadable urls found or unexpected format
    if (format_pos <=0){
        emit badNetworkResponse(0);
        return;
    }
    QString format=_url.sliced(format_pos+1);
    QString url=_url.chopped(_url.length()-format_pos);
         auto request = createRequest(url);
         auto reply = m_networkAccessManager.get(request);
         connect(reply,&QNetworkReply::redirected,this,[this,reply](const QUrl _rurl){ this->redirected(reply,_rurl); });

        connect(reply, &QNetworkReply::readyRead, this,
                 [this, reply, id, uuid,format] ()
                 {

                         if(api_error_helper::apiRequestFailed(reply, 200))
                         {
                             api_error_helper::logErrorMessage(
                                 reply, "Getting free book's media");
                            emit badNetworkResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
                             reply->deleteLater();
                             return;
                         }
                         emit gettingBookMediaChunkReady(id, uuid, reply->readAll(),
                                                         changeFormat(reply,format), false);
                 });

         connect(reply, &QNetworkReply::finished, this,
                 [this, reply, id, uuid,format]()
                 {
                    if(api_error_helper::apiRequestFailed(reply, 200))
                    {
                        api_error_helper::logErrorMessage(
                            reply, "Getting free book's media");
                        emit badNetworkResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
                        reply->deleteLater();
                        return;
                    }
                     emit gettingBookMediaChunkReady(id, uuid, QByteArray(),changeFormat(reply,format), true);
                     reply->deleteLater();
                 });

         connect(reply, &QNetworkReply::downloadProgress, this,
                 [this, id](qint64 bytesReceived, qint64 bytesTotal)
                 {
                     if (bytesTotal >0)
                         emit gettingBookMediaProgressChanged(id, bytesReceived,
                                                              bytesTotal);
                 });

         connect (reply, &QNetworkReply::errorOccurred, this, [this,reply](QNetworkReply::NetworkError code){
             emit badNetworkResponse(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
             reply->deleteLater();
             qDebug() << code;
         });

}

// alow all redirects
void OpdsAccess::redirected( QNetworkReply* const reply,const QUrl& url){
    qDebug() << url.toString();
    emit reply->redirectAllowed();
}

// change format depending on response headers
QString OpdsAccess::changeFormat(const QNetworkReply*   const reply,const QString& format) const{
    QString  content_type=QString( reply->rawHeader("content-type"));
    QString new_format;
    if (content_type.isEmpty()){
        qDebug() << "bad content type";
       return QString();
    }
    if (content_type.contains("octet-stream")){
        return format;
    }
    else if (content_type.contains("text/html")){
        return "html";
    }
    else if (content_type.contains("zip")){
        return format+".zip";
    }
    return QString();
}

} // namespace infrastructure::persistence
