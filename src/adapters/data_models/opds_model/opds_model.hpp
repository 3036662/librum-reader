#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QVariant>
#include <vector>
#include "adapters_export.hpp"
#include "opds_node.hpp"

namespace  adapters::data_models
{



class ADAPTERS_EXPORT OpdsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles
    {
        TitleRole,
        AuthorRole,
        UrlRole,
        DescriptionRole,
        IdRole,
        ImageUrlRole,
        imgDataReadyRole
    };

    OpdsModel() = default;
    OpdsModel(const std::vector<domain::value_objects::OpdsNode>* data );

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    public slots:
        void startedDataChange();
        void completedDataChange();
        void refreshNode(int row);

    signals:


private:
    const std::vector<domain::value_objects::OpdsNode>* m_data;
};

} // namespace  data_models
