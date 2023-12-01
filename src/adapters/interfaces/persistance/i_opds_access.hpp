#pragma once
#include <QObject>
#include "adapters_export.hpp"

namespace adapters
{


class ADAPTERS_EXPORT IOpdsAccess : public QObject
{
    Q_OBJECT

public:
    virtual ~IOpdsAccess() noexcept = default;
    virtual void loadRootLib(const QString& url) = 0;
    virtual void getOpdsImage(const QString& id, const QString& url) = 0;
    virtual void getBookMedia(const QString& id, const QUuid& uuid,
                              const QString& url) = 0;


signals:
    void loadOpdsRootFinished(const QByteArray& data);
    void gettingOpdsImageFinished(const QString& id, const QString& utl,
                                  const QByteArray& data);
    void badNetworkResponse(int code);
    void gettingBookMediaChunkReady(const QString& id, const QUuid& uuid,
                                    const QByteArray& data,
                                    const QString& format, bool isChunkLast);
    void gettingBookMediaProgressChanged(const QString& opdsId,
                                         qint64 bytesReceived,
                                         qint64 bytesTotal);
};


}  // namespace  adapters
