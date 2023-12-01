#pragma once
#include <QImage>
#include <QString>
#include <QVector>

namespace domain::value_objects
{

struct OpdsNode
{

    OpdsNode() = default;

    OpdsNode(const QString& _title, const QString& _author, const QString& _url,
             const QString& _descr, const QString& _id,
             const QString& _imageUrl, const QImage& _imageObj = QImage(),
             const bool _imgDataReady = false,
             const QVector<QPair<QString, QString>> _downloadUrls =
                 QVector<QPair<QString, QString>>(),
             double _mediaDownloadProgress = 0.0, bool _downloaded = false)
        : title(_title),
          author(_author),
          url(_url),
          descr(_descr),
          id { _id },
          imageUrl(_imageUrl),
          imageObj(_imageObj),
          imgDataReady(_imgDataReady),
          downloadUrls(_downloadUrls),
          mediaDownloadProgress(_mediaDownloadProgress),
          downloaded(_downloaded)
    {
    }

    QString title;
    QString author;
    QString url;
    QString descr;
    QString id;
    QString imageUrl;
    QImage imageObj;
    bool imgDataReady;
    QVector<QPair<QString, QString>> downloadUrls;
    double mediaDownloadProgress;
    bool downloaded;
};

}  // namespace domain::value_objects
