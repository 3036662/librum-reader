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
//    case ImageRole:
//    {
//        auto& image = opdsNode.image;
//        if(image.isNull())
//            return "";

//        QByteArray byteArray;
//        QBuffer buffer(&byteArray);
//        buffer.open(QIODevice::WriteOnly);
//        image.save(&buffer, "jpeg");
//        QString base64 = QString::fromUtf8(byteArray.toBase64());
//        return QString("data:image/jpeg;base64,") + base64;
//    }
//         break;
    case imgDataReadyRole:
        return opdsNode.imgDataReady;
        break;
    default:
        return QVariant();
        break;
    }
}

QHash<int, QByteArray> OpdsModel::roleNames() const {
    static QHash<int,QByteArray> roles{
                {TitleRole,"title"},
                 {UrlRole,"url"},
                {DescriptionRole,"descr"},
                {IdRole,"id"},
                {ImageUrlRole,"imageUrl"},
                {imgDataReadyRole,"imgDataReady"}
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

} //namespace adapters::data_models
