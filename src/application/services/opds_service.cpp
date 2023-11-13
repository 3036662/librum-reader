#include "opds_service.hpp"

namespace application::services {

OpdsService::OpdsService(IOpdsGateway*  opdsGateway)
    : m_opdsGateway(opdsGateway)
{

}



const std::vector<domain::value_objects::OpdsNode>&
OpdsService::getOpdsNodes() {
  loadRootNodesFromFile();
  return m_opdsNodes;
}

void OpdsService::loadRootNodesFromFile() {
  m_opdsNodes.clear();
  const QString localAppFolder =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  const QDir destDir(localAppFolder);
  if (!destDir.exists()) destDir.mkpath(".");
  const QString filepath = destDir.filePath("opds.json");
  QFile opdsFile(filepath);
  if (!opdsFile.exists()) {
    qDebug() << "Json file not found";
    m_opdsNodes.clear();
    return;
  }
  QJsonParseError jsonParseError;
  QJsonDocument jsonDoc;
  // open file
  if (opdsFile.open(QIODevice::ReadOnly)) {
    QByteArray bytesFromFile = opdsFile.readAll();
    if (bytesFromFile.isEmpty()) {
      qDebug() << "Opds libs json file is empty";
      return;
    }
    jsonDoc = QJsonDocument::fromJson(bytesFromFile, &jsonParseError);
    if (jsonDoc.isNull()) {
      qDebug() << "Can't parse opds libs json file. "
               << jsonParseError.errorString();
      return;
    }
  } else {
    qDebug() << "Can't open opds libs json file.";
    return;
  }
  opdsFile.close();
  const QJsonArray libsJsonArray = jsonDoc.array();
  if (libsJsonArray.isEmpty()) {
    qDebug() << "libs array is empty";
    return;
  }
  // create nodes
  for (auto it = libsJsonArray.cbegin(); it != libsJsonArray.cend(); ++it) {
    QJsonObject lib(it->toObject());
    if (lib.isEmpty()) continue;
    m_opdsNodes.emplace_back(lib.value("title").toString(),
                             lib.value("url").toString(),
                             lib.value("descr").toString());
  }
}


// load  entries and links for url
void OpdsService::loadRootLib(const QString& url){
  m_opdsGateway->loadRootlib(url);

}

}  // namespace application::services
