#pragma once
#include <QString>
#include <QImage>

namespace domain::value_objects {

struct OpdsNode {
  QString title;
  QString author;
  QString url;
  QString descr;
  QString id;
  QString imageUrl;
  QImage imageObj;
  bool imgDataReady;
};

}  // namespace domain::value_objects
