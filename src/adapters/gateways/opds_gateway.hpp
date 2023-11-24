#pragma once
#include <QUrl>
#include "adapters_export.hpp"
#include "i_opds_gateway.hpp"
#include "i_opds_access.hpp"

namespace  adapters::gateways {


class ADAPTERS_EXPORT OpdsGateway
    : public application::IOpdsGateway
{
    Q_OBJECT

public:
    OpdsGateway(IOpdsAccess* opdsAccess);

    void loadRootlib(QString url) override;
    void scaleImage(QImage& img) override;
public slots:
    void parseOpdsResonse(const QByteArray& data) override;
    void getOpdsImage(const QString& id,const QString& url) override;
    void getBookMedia(const QString& id,const QUuid& uuid ,const QString& url) override;


private:
    void convertRelativeUrlToAbsolute(QString& url);
    QString  convertRelativeUrlToAbsolute(const std::string& url);
    void processOpdsImage(const QString& id,const QString& url,const QByteArray& data);


    IOpdsAccess* m_OpdsAccess;
    QUrl baseurl;

    const int maxImgHeight=240;
    const int maxImgWidth=190;
};

} // namespace adapters::gateways
