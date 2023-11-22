#include "opds_gateway.hpp"
#include "opdsparser.hpp"
#include <webp/decode.h>
#include <QBuffer>
#include <iterator>


namespace adapters::gateways{
using namespace application::utility::opds ;
using namespace  domain::value_objects;


OpdsGateway::OpdsGateway(IOpdsAccess* opdsAccess)
    : m_OpdsAccess(opdsAccess){
    connect(m_OpdsAccess,
        &IOpdsAccess::loadOpdsRootFinished,this,
            &OpdsGateway::parseOpdsResonse);

    connect(m_OpdsAccess,
            &IOpdsAccess::gettingOpdsImageFinished,this,
            &OpdsGateway::processOpdsImage);

    connect(m_OpdsAccess,
            &IOpdsAccess::badNetworkResponse,this,
            &IOpdsGateway::badNetworkResponse);
}

//convert relative url to absolute
void OpdsGateway::convertRelativeUrlToAbsolute(QString& url){
    QUrl new_url(url.trimmed());
    if (!new_url.isRelative()) return;
    if (!new_url.path().startsWith("/")) new_url.setPath("/"+new_url.path());
    if (baseurl.isValid() && !baseurl.isEmpty() && !baseurl.isRelative() ){
        new_url.setScheme(baseurl.scheme());
        new_url.setHost(baseurl.host());

        url = new_url.toString();
    }
}
// overload for std::string url
QString  OpdsGateway::convertRelativeUrlToAbsolute(const std::string& url){
    QString res(url.c_str());
    convertRelativeUrlToAbsolute(res);
    return res;
}


// load OPDS Feed by url
void OpdsGateway::loadRootlib(QString url){
    QUrl new_url(url);
    if (new_url.isValid()  ){
        if (!new_url.isRelative())
            baseurl=url;
        else convertRelativeUrlToAbsolute(url);
    m_OpdsAccess->loadRootLib(url);
    }
}


void OpdsGateway::parseOpdsResonse(const QByteArray& data){
    OpdsParser parser( std::string (data.constData(),data.length()) );
    // fill vector with OpdsNodes
    std::vector<OpdsNode> res;
    for (auto it=parser.dom.entries.cbegin(); it!=parser.dom.entries.end(); ++it){
        if (it->title.empty()) continue; // skip entries with empty title
        // accumulate authors
        QString authors;
        for (auto it_author=it->authors.cbegin();it_author != it->authors.cend();++it_author){
            authors+=it_author->name;
            authors+=" ";
        }
        res.emplace_back(
            it->title.c_str(), // title
            authors, // author
            convertRelativeUrlToAbsolute(parser.getEntryUrlByID(it->id)), // url
            it->content.empty()  ? it->title.c_str() : it->content[0].text.c_str(), // content
            it->id.c_str(), // id
            parser.getImageUrlByID(it->id).empty() ? "" : convertRelativeUrlToAbsolute(parser.getImageUrlByID(it->id)), // imageUrl
             QImage(), // empty data
               false // imgDataReady
            );
    }
    emit parsingXmlDomCompleted(res);
}


void OpdsGateway::getOpdsImage(const QString& id,const QString& url){
   // if (boost::ends_with(url,".jpg") || boost::ends_with(url,".jpeg") ){
    m_OpdsAccess->getOpdsImage(id,url);
    //}
}

void OpdsGateway::processOpdsImage(const QString& id,const QString & url, const QByteArray& data){
    if (boost::ends_with(url,"webp")){
            int width, height;
            uint8_t* rgbData = WebPDecodeRGBA( reinterpret_cast<const uint8_t*>( data.constData()), data.size(), &width, &height);
            if (rgbData == nullptr) return;
            QImage image(rgbData, width, height, QImage::Format_RGBA8888);
            WebPFree(rgbData);
            if (!image.isNull()){
                scaleImage(image);
                emit gettingOpdsImagedFinished(id,image);
            }
    }
    else{
          QImage image;
          image.loadFromData(data);
          if (!image.isNull()){
          scaleImage(image);
           emit gettingOpdsImagedFinished(id,image);
          }
    }
}

void OpdsGateway::scaleImage(QImage& img){
    if (img.isNull()) return;
    if (img.height() > maxImgHeight || img.width() > maxImgWidth){
        img=img.scaled(QSize(maxImgWidth,maxImgHeight),Qt::KeepAspectRatio);
    }
}



} // namespace adapters::gateways
