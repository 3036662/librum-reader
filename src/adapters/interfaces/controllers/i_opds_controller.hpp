#pragma once
#include <QObject>
#include <opds_model.hpp>

class IOpdsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(
        adapters::data_models::OpdsModel* opdsModel READ getOpdsModel CONSTANT)

public:
    virtual ~IOpdsController() noexcept = default;
    virtual adapters::data_models::OpdsModel* getOpdsModel() = 0;
    virtual void handleBadNetworkResponse(int code) = 0;
    Q_INVOKABLE virtual void loadRootLib(const QString& url) = 0;
    Q_INVOKABLE virtual void getNodeImage(const QString& id) = 0;
    Q_INVOKABLE virtual void getBookMedia(const QString& id,
                                          const QString& downloadUrl) = 0;
    Q_INVOKABLE virtual void markBookAsDownloaded(const QString& id) = 0;
    Q_INVOKABLE virtual void saveOpdsLib(const QString& title,
                                         const QString& url,
                                         const QString desr) = 0;
    // Q_INVOKABLE virtual void deleteNodeImage(const QString& id)=0;
signals:
    void badOpdsLibData(const QString& err);
    void savingLibFinished();
    void badNetworkResponse(int code);
    void gettingBookFinished(const QString& path, const QString& id);
};
