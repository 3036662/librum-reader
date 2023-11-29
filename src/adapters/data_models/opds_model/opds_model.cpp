#include "opds_model.hpp"
#include <QBuffer>

using namespace domain::value_objects;

namespace adapters::data_models
{

OpdsModel::OpdsModel(const std::vector<domain::value_objects::OpdsNode> *data):
    m_data(data)
{
}

int OpdsModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_data->size();
}

QVariant OpdsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    const domain::value_objects::OpdsNode& opdsNode = m_data->at(index.row());

    switch (role) {
    case TitleRole:
        return opdsNode.title;
        break;
    case AuthorRole:
        return opdsNode.author;
        break;
    case UrlRole:
        return opdsNode.url;
        break;
    case DescriptionRole:
        return opdsNode.descr;
         break;
    case IdRole:
         return opdsNode.id;
         break;
    case ImageUrlRole:
        return opdsNode.imageUrl;
        break;
    case imgDataReadyRole:
        return opdsNode.imgDataReady;
        break;
    case downloadUrlRole:{
        QString url;
        //look for epub
        for (auto it = opdsNode.downloadUrls.constBegin(); it !=opdsNode.downloadUrls.constEnd(); ++it){
            if (it->first.contains("epub")){
                url=it->second+"_epub";
            }
        }
        // look for pdf
        if (url.isEmpty()){
            for (auto it = opdsNode.downloadUrls.constBegin(); it !=opdsNode.downloadUrls.constEnd(); ++it){
                if (it->first.contains("pdf")){
                    url=it->second+"_pdf";
                }
            }
        }
        // look for fb2
        if (url.isEmpty()){
            for (auto it = opdsNode.downloadUrls.constBegin(); it !=opdsNode.downloadUrls.constEnd(); ++it){
                if (it->first.contains("fb2")){
                    url=it->second+"_fb2";
                }
            }
        }
        // look for djvu
        if (url.isEmpty()){
            for (auto it = opdsNode.downloadUrls.constBegin(); it !=opdsNode.downloadUrls.constEnd(); ++it){
                if (it->first.contains("djvu")){
                    url=it->second+"_djvu";
                }
            }
        }
        // look for html
        if (url.isEmpty()){
            for (auto it = opdsNode.downloadUrls.constBegin(); it !=opdsNode.downloadUrls.constEnd(); ++it){
                if (it->first.contains("html")){
                    url=it->second+"_html";
                }
            }
        }
        if (url.isEmpty()  && !opdsNode.downloadUrls.isEmpty() ){
            url=opdsNode.downloadUrls.first().second;
        }
        return url;
        }
        break;
    case MediaDownloadProgressRole:
        return opdsNode.mediaDownloadProgress;
    case DownloadedRole:
        return opdsNode.downloaded;
    default:
        return QVariant();
        break;
    }
}

QHash<int, QByteArray> OpdsModel::roleNames() const {
    static QHash<int,QByteArray> roles{
                {TitleRole,"title"},
                {AuthorRole,"author"},
                 {UrlRole,"url"},
                {DescriptionRole,"descr"},
                {IdRole,"id"},
                {ImageUrlRole,"imageUrl"},
                {imgDataReadyRole,"imgDataReady"},
                {downloadUrlRole,"downloadUrl"},
                { MediaDownloadProgressRole, "mediaDownloadProgress" },
                { DownloadedRole, "downloaded" },
    };
    return roles;
}

void OpdsModel::startedDataChange(){
    beginResetModel();
}

void OpdsModel::completedDataChange(){
    endResetModel();
}

void OpdsModel::refreshNode(int row){
    dataChanged(index(row,0),index(row,0));
}

void OpdsModel::downloadingBookMediaProgressChanged(int row){
    emit dataChanged(index(row, 0), index(row, 0),
                     { MediaDownloadProgressRole });
}

void OpdsModel::bookIsDownloadedChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0), { DownloadedRole });
}

} //namespace adapters::data_models
