#include "opds_model.hpp"

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
    };
    return roles;
}

} //namespace adapters::data_models
