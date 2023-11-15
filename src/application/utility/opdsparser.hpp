#pragma once
#include "opds_structs.hpp"
#include <tinyxml2.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <iomanip>
#include <iostream>
#include <string>



namespace application::utility::opds
{


class OpdsParser {
public:
    OpdsParser() = delete;
    OpdsParser(const std::string& body);
    void parse(std::string body);
    std::vector<Link> parseLinks(const tinyxml2::XMLElement* const el) const;
    std::vector<Content> parseContents(const tinyxml2::XMLElement* const el) const;
    std::vector<Author> parseAuthors(const tinyxml2::XMLElement* const el) const;
    std::vector<Entry> parseEntries(const tinyxml2::XMLElement* const el) const;
    std::vector<Entry>::const_iterator getIteratortoEntryById(const std::string& id) const;
    std::string getEntryUrlByID(const std::string& id) const;
    std::string getImageUrlByID(const std::string& id) const;
    std::string checkUrl(const std::string& url) const;
    void clearDom();
    tinyxml2::XMLDocument doc;
    Feed dom;
};

} // namespace application::utility::opds
