#include "opds_model.hpp"
#include "opds_structs.hpp"
#include <QBuffer>

using namespace domain::value_objects;
using namespace application::utility::opds;

namespace adapters::data_models
{



OpdsModel::OpdsModel(const std::vector<domain::value_objects::OpdsNode>* data)
    : m_data(data)
{
}

int OpdsModel::rowCount(const QModelIndex& parent) const
{
    if(parent.isValid())
        return 0;

    return m_data->size();
}

QVariant OpdsModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();
    const domain::value_objects::OpdsNode& opdsNode = m_data->at(index.row());

    switch(role)
    {
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
    case downloadUrlRole:
    {
        QString url;
        if(url.isEmpty() && !opdsNode.downloadUrls.isEmpty())
        {
            // first try to return epub
            auto it_download_link =std::find_if( opdsNode.downloadUrls.constBegin(),
                                                 opdsNode.downloadUrls.constEnd(),[](const QPair<QString, QString>& link){
                                                     return link.first.contains("epub");
                                                 });
            if (it_download_link!= opdsNode.downloadUrls.constEnd())
                return it_download_link->second;
            // if not found  -pdf
             it_download_link =std::find_if( opdsNode.downloadUrls.constBegin(),
                                                 opdsNode.downloadUrls.constEnd(),[](const QPair<QString, QString>& link){
                                                     return link.first.contains("pdf");
                                                 });
            if (it_download_link!= opdsNode.downloadUrls.constEnd())
                return it_download_link->second;
             // if not found  -  fb2
             it_download_link =std::find_if( opdsNode.downloadUrls.constBegin(),
                                                 opdsNode.downloadUrls.constEnd(),[](const QPair<QString, QString>& link){
                                                     return link.first.contains("fb2");
                                                 });
            if (it_download_link!= opdsNode.downloadUrls.constEnd())
                return it_download_link->second;
          // if nothing was found
          url = opdsNode.downloadUrls.first().second;
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

QHash<int, QByteArray> OpdsModel::roleNames() const
{
    static QHash<int, QByteArray> roles {
        { TitleRole, "title" },
        { AuthorRole, "author" },
        { UrlRole, "url" },
        { DescriptionRole, "descr" },
        { IdRole, "id" },
        { ImageUrlRole, "imageUrl" },
        { imgDataReadyRole, "imgDataReady" },
        { downloadUrlRole, "downloadUrl" },
        { MediaDownloadProgressRole, "mediaDownloadProgress" },
        { DownloadedRole, "downloaded" },
    };
    return roles;
}

void OpdsModel::startedDataChange()
{
    beginResetModel();
}

void OpdsModel::completedDataChange()
{
    endResetModel();
}

void OpdsModel::refreshNode(int row)
{
    dataChanged(index(row, 0), index(row, 0));
}

void OpdsModel::downloadingBookMediaProgressChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0),
                     { MediaDownloadProgressRole });
}

void OpdsModel::bookIsDownloadedChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0), { DownloadedRole });
}

}  // namespace adapters::data_models
