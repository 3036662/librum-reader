#pragma once
#include <QImage>
#include <QObject>
#include <QUuid>
#include "application_export.hpp"
#include "opds_node.hpp"

namespace application
{

class APPLICATION_EXPORT IOpdsGateway : public QObject
{
    Q_OBJECT

public:
    virtual ~IOpdsGateway() noexcept = default;

    virtual void loadRootlib(QString url) = 0;
    virtual void getOpdsImage(const QString& id, const QString& url) = 0;
    virtual void scaleImage(QImage& img) = 0;
    virtual void getBookMedia(const QString& id, const QUuid& uuid,
                              const QString& url) = 0;


public slots:
    virtual void parseOpdsResonse(const QByteArray& data) = 0;

signals:
    void parsingXmlDomCompleted(
        const std::vector<domain::value_objects::OpdsNode>&);
    void gettingOpdsImagedFinished(const QString& id, const QImage& image);
    void badNetworkResponse(int code);
    void gettingBookMediaChunkReady(const QString& id, const QUuid& uuid,
                                    const QByteArray& data,
                                    const QString& format, bool isChunkLast);
    void gettingBookMediaProgressChanged(const QString& id,
                                         qint64 bytesReceived,
                                         qint64 bytesTotal);
};


}  // namespace application
