#include "opdsparser.hpp"
#include <cstring>

namespace application::utility::opds
{


OpdsParser::OpdsParser(const std::string& body) : doc(tinyxml2::XMLDocument()) {
    parse(body);
}

// parse plain xml  to build OpdsParser.dom (Feed object)
void OpdsParser::parse(std::string body) {
    // erase all before and after response
    doc.Clear();
    body.erase(0, body.find("<feed"));
    body.erase(boost::algorithm::find_last(body, "</feed>").end(), body.end());
    // workaround for coollib weird behaviour
    //   boost::algorithm::erase_all_regex(body, boost::regex("\\r\\n.{0,5}\\r\\n"));

    if (body.empty()) {
        std::cerr << "xml body is empty" << std::endl;
        return;
    }
    if (doc.Parse(body.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error parsing xml data" << std::endl;
        std::cout << body.c_str();
        return;
    }
    tinyxml2::XMLElement* xmlEntry = doc.RootElement();

    const tinyxml2::XMLElement* tempXmlPtr;
    const char* tempCharPtr;
    if ((tempXmlPtr = xmlEntry->FirstChildElement("updated")) &&
       (tempCharPtr = tempXmlPtr->GetText())) {
        dom.updated = tempCharPtr;
    }
    if ((tempXmlPtr = xmlEntry->FirstChildElement("id")) &&
       (tempCharPtr = tempXmlPtr->GetText())) {
        dom.id = tempCharPtr;
    }
    if ((tempXmlPtr = xmlEntry->FirstChildElement("icon")) &&
       (tempCharPtr = tempXmlPtr->GetText())) {
        dom.icon = tempCharPtr;
    }
    if ((tempXmlPtr = xmlEntry->FirstChildElement("title")) &&
       (tempCharPtr = tempXmlPtr->GetText())) {
        dom.icon = tempCharPtr;
    }
    // process links
    std::cerr << "Process root links" << std::endl;
    dom.links = parseLinks(xmlEntry);
    // process enries
    std::cerr << "Process root entries" << std::endl;
    dom.entries = parseEntries(xmlEntry);
}

std::vector<Link> OpdsParser::parseLinks(const tinyxml2::XMLElement* const el) const {
    std::vector<Link> res;
    const tinyxml2::XMLElement* xmlLink = el->FirstChildElement("link");
    const tinyxml2::XMLElement* xmlLinkLast = el->LastChildElement("link");
    while (xmlLink != nullptr) {
        res.emplace_back(
            xmlLink->Attribute("rel") != nullptr ? xmlLink->Attribute("rel") : "",
            xmlLink->Attribute("href") != nullptr ? xmlLink->Attribute("href") : "",
            xmlLink->Attribute("type") != nullptr ? xmlLink->Attribute("type")
                                                  : "");
        if (xmlLink == xmlLinkLast)
            break;
        else
            xmlLink = xmlLink->NextSiblingElement();
    }
    return res;
}

std::vector<Entry> OpdsParser::parseEntries(
    const tinyxml2::XMLElement* const el) const{
    std::vector<Entry> res;
    const tinyxml2::XMLElement* xmlEntry = el->FirstChildElement("entry");
    const tinyxml2::XMLElement* xmlEntryLast = el->LastChildElement("entry");
    while (xmlEntry) {
        Entry entry;
        const tinyxml2::XMLElement* tempXmlPtr;
        const char* tempCharPtr;
        if ((tempXmlPtr = xmlEntry->FirstChildElement("updated")) &&
           (tempCharPtr = tempXmlPtr->GetText())) {
            entry.updated = tempCharPtr;
        }
        if ((tempXmlPtr = xmlEntry->FirstChildElement("id")) &&
           (tempCharPtr = tempXmlPtr->GetText())) {
            entry.id = tempCharPtr;
        }
        if ((tempXmlPtr = xmlEntry->FirstChildElement("title")) &&
           (tempCharPtr = tempXmlPtr->GetText())) {
            entry.title = tempCharPtr;
        }
        entry.links = parseLinks(xmlEntry);
        entry.content = parseContents(xmlEntry);
        entry.authors = parseAuthors(xmlEntry);
        //check if entry is empty if not -> push to result
        if  (!entry.title.empty()  && !entry.id.empty()   &&    (!entry.authors.empty() ||  !entry.content.empty()  ||! entry.links.empty()))
                res.push_back(std::move(entry));
        if (xmlEntry == xmlEntryLast)
            break;
        else
            xmlEntry = xmlEntry->NextSiblingElement();
    }
    return res;
}

std::vector<Content> OpdsParser::parseContents(
    const tinyxml2::XMLElement* const el) const{
    std::vector<Content> res;
    const tinyxml2::XMLElement* xmlContent = el->FirstChildElement("content");
    const tinyxml2::XMLElement* xmlContentLast = el->LastChildElement("content");
    while (xmlContent) {
        if (xmlContent->Attribute("type") && strcmp(xmlContent->Attribute("type"),"text")==0 ){ // content must use type="text" by standart
            res.emplace_back(
                "text", xmlContent->GetText() != nullptr ? xmlContent->GetText() : "");
        }
        if (xmlContent == xmlContentLast)
            break;
        else
            xmlContent = xmlContent->NextSiblingElement();
    }
    return res;
}

std::vector<Author> OpdsParser::parseAuthors (
    const tinyxml2::XMLElement* const el) const {
    std::vector<Author> res;
    const tinyxml2::XMLElement* xmlAuthor = el->FirstChildElement("author");
    const tinyxml2::XMLElement* xmlAuthorLast = el->LastChildElement("author");
    while (xmlAuthor) {
        Author author;
        const tinyxml2::XMLElement* tempXmlPtr;
        const char* tempCharPtr;
        if ((tempXmlPtr = xmlAuthor->FirstChildElement("name")) &&
           (tempCharPtr = tempXmlPtr->GetText())) {
            author.name = tempCharPtr;
        }
        if ((tempXmlPtr = xmlAuthor->FirstChildElement("uri")) &&
           (tempCharPtr = tempXmlPtr->GetText())) {
            author.uri = tempCharPtr;
        }
        res.push_back(std::move(author));
        if (xmlAuthor == xmlAuthorLast)
            break;
        else
            xmlAuthor = xmlAuthor->NextSiblingElement();
    }
    return res;
}

// return iterator to entry by id
std::vector<Entry>::const_iterator OpdsParser::getIteratortoEntryById(const std::string& id) const{
    return id.empty() ?  dom.entries.cend() : std::find_if(dom.entries.cbegin(), dom.entries.cend(),
                 [&id](const Entry& entry) { return entry.id == id; });
}


// return  entry browsable url by id
std::string OpdsParser::getEntryUrlByID(const std::string& id) const {
    std::string res;
    if (id.empty())
        return res;
    std::vector<Entry>::const_iterator found = getIteratortoEntryById(id);
    // return browsable url of entry or empty string if nothing found;
    if (found != dom.entries.end()) {
        auto  found_url= std::find_if (found->links.cbegin(), found->links.cend(),  [](const Link& link){
            // link type must be atom+xml and rel != relative
            return boost::algorithm::contains(link.type,"atom+xml") && !boost::contains(link.rel,"related");
            });
        if (found_url != found->links.cend())
            res = found_url->href;
        boost::algorithm::trim(res);
    }
    return res;
}

//get enty image link if set
std::string OpdsParser::getImageUrlByID(const std::string& id) const{
    std::string res;
    auto entry_it = getIteratortoEntryById(id);
    if (entry_it != dom.entries.cend()){

        // if not found try to find simple image

        auto image_it=  std::find_if(entry_it->links.cbegin(),
                                    entry_it->links.cend(),[](const Link& link){
                                        return boost::algorithm::contains(link.rel,"image") && boost::algorithm::contains(link.type,"image");
                                    });
         // try to find thumbnail
        if (image_it == entry_it->links.cend()){
            image_it=  std::find_if(entry_it->links.cbegin(),
                                         entry_it->links.cend(),[](const Link& link){
                                             return boost::algorithm::contains(link.rel,"thumbnail") && boost::algorithm::contains(link.type,"image");
                                         });
        }
        // try to find only with  rel if nothing found
        if (image_it == entry_it->links.cend()){
            image_it=  std::find_if(entry_it->links.cbegin(),
                                    entry_it->links.cend(),[](const Link& link){
                                        return boost::algorithm::contains(link.rel,"thumbnail") ||  boost::algorithm::contains(link.rel,"image");
                                    });
        }

        // if found
        if (image_it != entry_it->links.cend()){
            res=image_it->href;
        }
    }
    return res;
}

// check if url is browsable (atom+xml)
std::string OpdsParser::checkUrl(const std::string& url) const {
    // look in current feed
    std::vector<Link>::const_iterator found = std::find_if(
        dom.links.cbegin(), dom.links.cend(), [&url](const Link& link) {
            return link.href == url &&
                   boost::algorithm::contains(link.type, "atom+xml");
        });
    if (found != dom.links.end()) {
        return url;
    }
    // try to look for link in entries
    bool goodlink{false};
    for (auto it = dom.entries.cbegin(); it != dom.entries.cend(); ++it) {
        for (auto itLink = it->links.cbegin(); itLink != it->links.cend(); ++itLink) {
            goodlink = itLink->href == url &&
                       boost::algorithm::contains(itLink->type, "atom+xml");
            if (goodlink) break;
        }
        if (goodlink) break;
    }
    return goodlink ? url : "";
}

void OpdsParser::clearDom() { dom = Feed(); }


} // namespace application::utility::opds
