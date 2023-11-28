#pragma once
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QVector>
#include "i_opds_gateway.hpp"
#include "i_opds_service.hpp"

namespace application::services
{

using namespace domain::value_objects;

class APPLICATION_EXPORT OpdsService : public IOpdsService
{
public:
    OpdsService(IOpdsGateway* opdsGateway);

    const std::vector<OpdsNode>& getOpdsNodes() override;
    void loadRootLib(const QString& url) override;
    void getNodeImage(const QString& id) override;
    void getBookMedia(const QString& id, const QString& url) override;
    void markBookAsDownloaded(const QString& id) override;
    // void deleteNodeImage(const QString &id) override;

public slots:
    void setupUserData(const QString& token, const QString& email) override;
    void clearUserData() override;
    void processNodes(const std::vector<OpdsNode>&) override;
    void setOpdsNodeCover(const QString& id, const QImage& data);

private slots:
    void saveDownloadedBookMediaChunkToFile(const QString& opdsId,
                                            const QUuid& uuid,
                                            const QByteArray& data,
                                            const QString& format,
                                            bool isLastChunk);

private:
    IOpdsGateway* m_opdsGateway;
    void loadRootNodesFromFile();
    const OpdsNode findNodeByUrl(const QString& url) const;
    const QImage* getImageDataByImgUrl(const QString& imgUrl) const override;
    QDir getLibraryDir() const;
    QString getUserLibraryName(const QString& email) const;
    void setMediaDownloadProgressForBook(const QString& id,
                                         qint64 bytesReceived,
                                         qint64 bytesTotal);


    std::vector<OpdsNode> m_opdsNodes;
    std::vector<OpdsNode> historyStack;
    QString m_userEmail;
    const OpdsNode rootNode { "/", "", "url_root", "Root", "url_root", "" };
};

}  // namespace application::services
