#pragma once
#include <QString>

namespace domain::value_objects {

struct OpdsNode {
  QString title;
  QString url;
  QString descr;
  QString id;
};

}  // namespace domain::value_objects
