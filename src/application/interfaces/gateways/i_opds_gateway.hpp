#pragma once
#include <QObject>
#include <QImage>
#include "opds_node.hpp"

#include "application_export.hpp"


namespace application {

class APPLICATION_EXPORT IOpdsGateway: public QObject{
    Q_OBJECT

public:
    virtual ~IOpdsGateway() noexcept =default;

    virtual void loadRootlib(QString url) = 0;
    virtual void getOpdsImage(const QString& id,const QString& url)=0;

public slots:
    virtual void parseOpdsResonse(const QByteArray& data)=0;

signals:
    void parsingXmlDomCompleted(const std::vector<domain::value_objects::OpdsNode>&  );
    void gettingOpdsImagedFinished(const QString& id,const QImage& image);
};


} // namespace application
