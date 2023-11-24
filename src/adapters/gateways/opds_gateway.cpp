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
    //send to service
    connect(m_OpdsAccess,
        &IOpdsAccess::gettingBookMediaChunkReady,this,
            &IOpdsGateway::gettingBookMediaChunkReady);

    connect(m_OpdsAccess,
            &IOpdsAccess::gettingBookMediaProgressChanged, this,
            &IOpdsGateway::gettingBookMediaProgressChanged);
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
    if (url.empty()) return QString();
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
        // construt donload links
        QVector<QPair<QString,QString>> downloadUrls;
        std::vector<std::pair<std::string,std::string>> link_vec=parser.getDownloadUrlsByID(it->id);
        for(auto it_dLink=link_vec.cbegin();it_dLink != link_vec.cend(); ++it_dLink){
            downloadUrls.append(QPair(QString(it_dLink->first.c_str()),
                                      convertRelativeUrlToAbsolute(it_dLink->second.c_str())) );
        }
        res.emplace_back(
            it->title.c_str(), // title
            std::move(authors), // author
            convertRelativeUrlToAbsolute(parser.getEntryUrlByID(it->id) ), // url
            it->content.empty()  ? it->title.c_str() : it->content[0].text.c_str(), // content
            it->id.c_str(), // id
            parser.getImageUrlByID(it->id).empty() ? "" : convertRelativeUrlToAbsolute(parser.getImageUrlByID(it->id)), // imageUrl
            QImage(),
            false, // image ready flag,
            std::move(downloadUrls) // download urls
            );
    }

    // search  result arrays for dublicates
    for (auto it = res.begin(); it != res.end(); ++it){
        auto it2 = std::find_if(it,res.end(), [&it](const OpdsNode& node1){
            if (   it->title == node1.title && it->author == it->author && it->imageUrl == node1.imageUrl && it->id!=node1.id){
                     //TODO merge urls to node
                    return true;
            }
            else return false;
        });
        // delete duplicate after meging
        if (it2 != res.end() &&  it2 != it){
            res.erase(it2);
        }
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

void  OpdsGateway::getBookMedia(const QString& id,const QUuid& uuid ,const QString& url){
        m_OpdsAccess->getBookMedia(id,uuid,url);
}



} // namespace adapters::gateways
