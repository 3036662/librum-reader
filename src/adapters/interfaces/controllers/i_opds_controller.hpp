#pragma once
#include <QObject>
#include <opds_model.hpp>

class IOpdsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(adapters::data_models::OpdsModel* opdsModel READ getOpdsModel CONSTANT)

public:
    virtual ~IOpdsController() noexcept = default;
    virtual adapters::data_models::OpdsModel*  getOpdsModel() =0;
    virtual void handleBadNetworkResponse(int code)=0;
    Q_INVOKABLE  virtual  void loadRootLib(const QString& url)=0;
    Q_INVOKABLE virtual void getNodeImage(const QString& id)=0;
    //Q_INVOKABLE virtual void deleteNodeImage(const QString& id)=0;
signals:
    void badNetworkResponse(int code);

};

