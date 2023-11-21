#include "opds_image_provider.hpp"
#include <boost/algorithm/string.hpp>

namespace application::utility::opds {

OpdsImageProvider::OpdsImageProvider(IOpdsService* opdsService):
    QQuickImageProvider(QQmlImageProviderBase::Image),ptrService(opdsService)
{
}

QImage OpdsImageProvider::requestImage(const QString &url, QSize *size, const QSize &requestedSize){

        const  QImage* ptrImg = ptrService->getImageDataByImgUrl(url);
        if (!ptrImg) return QImage();
        QImage result(*ptrImg);  //copy original image
        //check if scaling is needed
        if (!result.isNull() && requestedSize.height() > 80 &&   requestedSize.width() > 50
            && ( requestedSize.height() < result.height()   || requestedSize.width() < result.width() ) ){
           result=result.scaled(requestedSize, Qt::KeepAspectRatio);
        }
        if (!result.isNull()){
            size->setHeight(result.height());
            size->setWidth(result.width());
        }
        return result;
}
} // namespace application::utility::opds
