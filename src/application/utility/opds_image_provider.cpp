#include "opds_image_provider.hpp"
#include <boost/algorithm/string.hpp>

namespace application::utility::opds {

OpdsImageProvider::OpdsImageProvider(IOpdsService* opdsService):
    QQuickImageProvider(QQmlImageProviderBase::Image),ptrService(opdsService)
{
}

QImage OpdsImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
        // TODO
        // find node in nodes with requested id
        // prepate qImage
        // return qImage

        if (boost::ends_with(id,"webp")){
            qWarning()<<"webp";
        }
        QImage result;
       const  QByteArray* ptrData = ptrService->getImageDataByImgUrl(id);
        if (!ptrData) return QImage();
        result.loadFromData( *ptrData);
        if (!result.isNull() && requestedSize.height() > 80 &&   requestedSize.width() > 50 && requestedSize.height() < result.height() ){
            result=result.scaled(requestedSize, Qt::KeepAspectRatio);
        }
        if (!result.isNull()){
        size->setHeight(result.height());
        size->setWidth(result.width());
        }
        return result;
}
} // namespace application::utility::opds
