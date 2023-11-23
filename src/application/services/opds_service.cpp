#include "opds_service.hpp"
#include <algorithm>
#include <functional>

namespace application::services {



OpdsService::OpdsService(IOpdsGateway*  opdsGateway)
    : m_opdsGateway(opdsGateway)
{
    connect(
        m_opdsGateway,&IOpdsGateway::parsingXmlDomCompleted,
        this, &IOpdsService::processNodes);

    connect(
        m_opdsGateway,&IOpdsGateway::gettingOpdsImagedFinished,
        this, &OpdsService::setOpdsNodeCover);

    connect(m_opdsGateway,&IOpdsGateway::badNetworkResponse,
         this,&IOpdsService::badNetworkResponse);

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
                             "", // author
                             lib.value("url").toString(), // url
                             lib.value("descr").toString(), // descr
                             "", ""
                             );
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

//process nodes  recieved from the Gateway
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

const OpdsNode OpdsService::findNodeByUrl(const QString& url) const{
  auto it=std::find_if(m_opdsNodes.cbegin(),m_opdsNodes.cend(), [&url](const OpdsNode& node){
                    return node.url == url;
               });
  return it  == m_opdsNodes.end() ? OpdsNode() : *it;
}


void OpdsService::getNodeImage(const QString& id){
    auto itNode= std::find_if(m_opdsNodes.begin(),m_opdsNodes.end(), [&id](const OpdsNode& node){
        return node.id == id;
    });
    // if found and has imageUrl
    if (itNode != m_opdsNodes.end() && !(itNode->imageUrl).isEmpty() ){
        // TODO Handle data uri
        // for data uri
        if (itNode->imageUrl.startsWith("data:")){
            QByteArray data=itNode->imageUrl.toUtf8();
            qsizetype start_index=itNode->imageUrl.indexOf(',');
            if (start_index>0){
                    data.remove(0,start_index+1);
                    data=data.fromBase64(data);
                    if (!data.isEmpty()){
                        QImage image;
                        image.loadFromData(data);
                        if (!image.isNull()){
                            m_opdsGateway->scaleImage(image);
                            itNode->imageObj=std::move(image);
                            itNode->imageUrl=std::to_string(std::hash<QString>{}( itNode->imageUrl)).c_str();
                            itNode->imgDataReady=true;
                            emit dataChanged(std::distance(m_opdsNodes.begin(),  itNode));
                        }
                    }
            }
        }
        m_opdsGateway->getOpdsImage(id,itNode->imageUrl);
    }
}

/*
void OpdsService::deleteNodeImage(const QString &id){
    auto itNode= std::find_if(m_opdsNodes.begin(),m_opdsNodes.end(), [&id](const OpdsNode& node){
        return node.id == id;
    });
    // if found
    if (itNode != m_opdsNodes.end()){
        itNode->imageObj=QImage();
    };
}*/



void  OpdsService::setOpdsNodeCover(const QString& id, const QImage &data){
    auto itNode= std::find_if(m_opdsNodes.begin(),m_opdsNodes.end(), [&id](const OpdsNode& node){
        return node.id == id;
    });

    // if node was not found
    if (itNode == m_opdsNodes.end()) return;
    // if found - set image
    itNode->imageObj = std::move(data);
    itNode->imgDataReady = true;
    emit dataChanged(std::distance(m_opdsNodes.begin(),  itNode));
}

const QImage*   OpdsService::getImageDataByImgUrl(const QString& imgUrl) const {
    auto itNode= std::find_if(m_opdsNodes.cbegin(),m_opdsNodes.cend(), [&imgUrl](const OpdsNode& node){
        return node.imageUrl == imgUrl;
    });
    return itNode != m_opdsNodes.cend() && itNode->imgDataReady  ?  &itNode->imageObj : nullptr;
}


}  // namespace application::services
