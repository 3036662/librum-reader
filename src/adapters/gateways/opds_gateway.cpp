#include "opds_gateway.hpp"
#include "opdsparser.hpp"


namespace adapters::gateways{
using namespace application::utility::opds ;
using namespace  domain::value_objects;


OpdsGateway::OpdsGateway(IOpdsAccess* opdsAccess)
    : m_OpdsAccess(opdsAccess){
    connect(m_OpdsAccess,
        &IOpdsAccess::loadOpdsRootFinished,this,
            &OpdsGateway::parseOpdsResonse);

}

void OpdsGateway::loadRootlib(QString url){
    QUrl new_url(url);
    if (new_url.isValid() ){
        if (!new_url.isRelative())
            baseurl=url;
        else  // new url is relative
            if (baseurl.isValid() && !baseurl.isEmpty() && !baseurl.isRelative() ){
                new_url.setScheme(baseurl.scheme());
                new_url.setHost(baseurl.host());
                url = new_url.toString();
            }
    }

    m_OpdsAccess->loadRootLib(url);
}

void OpdsGateway::parseOpdsResonse(const QByteArray& data){
    OpdsParser parser(
        std::string (data.constData(),data.length())
        );
    // fill vector ov OpdsNodes
    std::vector<OpdsNode> res;
    for (auto it=parser.dom.entries.cbegin(); it!=parser.dom.entries.end(); ++it){
        res.emplace_back(
            it->title.c_str(),
            parser.getEntryUrlByID(it->id).c_str(),
            it->content.empty()  ?  "" : it->content[0].text.c_str(),
            it->id.c_str()
            );
    }
    emit parsingXmlDomCompleted(res);
}

} // namespace adapters::gateways
