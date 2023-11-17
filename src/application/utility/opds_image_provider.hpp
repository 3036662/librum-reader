#pragma once
#include <QQuickImageProvider>
#include <QImage>
#include "i_opds_service.hpp"


namespace application::utility::opds {

class OpdsImageProvider: public QQuickImageProvider
{
public:
   explicit  OpdsImageProvider(IOpdsService* opdsService);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
private:
   IOpdsService* ptrService;

};

} // namespace application::utility::opds
