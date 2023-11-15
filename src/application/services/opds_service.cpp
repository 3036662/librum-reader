#include "opds_service.hpp"
#include <algorithm>

namespace application::services {



OpdsService::OpdsService(IOpdsGateway*  opdsGateway)
    : m_opdsGateway(opdsGateway)
{
    connect(
        m_opdsGateway,&IOpdsGateway::parsingXmlDomCompleted,
        this, &IOpdsService::processNodes);

}



const std::vector<OpdsNode>&   OpdsService::getOpdsNodes() {
  historyStack.push_back(rootNode);
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
   // go to root -> load from json file
  if (url.isEmpty())
    return;
  if (url== "url_root"){
    emit  nodesVecReplaceStarted();
    historyStack.clear();
    historyStack.push_back(rootNode);
    m_opdsNodes.clear();
    loadRootNodesFromFile();
    emit opdsNodesReady();
    return;
  }
  // go back remove current node from history
  if (historyStack.size()>=2 &&   historyStack[historyStack.size()-2].url==url ){
        historyStack.pop_back();
  }
  //  push current to history
  else {
    OpdsNode  curr_target=findNodeByUrl(url);
    curr_target.title="..";
    historyStack.push_back(curr_target);
  }
  m_opdsGateway->loadRootlib(url);
}

//some nodes are recieved from the Gateway
void OpdsService::processNodes
    (const std::vector<OpdsNode>& nodes_vec) {
  emit  nodesVecReplaceStarted();
  m_opdsNodes.clear();
  if (historyStack.size()>=2){
    m_opdsNodes.push_back(rootNode);
    OpdsNode backNode=historyStack[historyStack.size()-2];
    if (rootNode.url != backNode.url )
        m_opdsNodes.push_back(backNode);
  }
  std::copy(nodes_vec.cbegin(),nodes_vec.cend(),std::back_inserter(m_opdsNodes));
  emit opdsNodesReady();
}

OpdsNode OpdsService::findNodeByUrl(const QString& url) const{
  auto it=std::find_if(m_opdsNodes.cbegin(),m_opdsNodes.cend(), [&url](const OpdsNode& node){
                    return node.url == url;
               });
  return it  == m_opdsNodes.end() ? OpdsNode() : *it;
}


void OpdsService::getNodeImage(const QString& id){

        // TODO
        // 1. find node with this id
        // 2. get  image url
        // 3. path it to gateway for downloading
}

}  // namespace application::services
