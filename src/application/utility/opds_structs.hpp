#pragma once
#include <iomanip>
#include <string>
#include <vector>

namespace application::utility::opds
{

// simplest struct <author>
struct Author
{
    Author() = default;
    Author(const std::string& name_, const std::string& uri);
    std::string name;
    std::string uri;
};

// simplest struct <content>
struct Content
{
    Content(const std::string&, const std::string&);
    std::string type;
    std::string text;
};

// simplest struct <link>
struct Link
{
    Link(const std::string& rel_, const std::string& href_,
         const std::string& type_);
    std::string rel;
    std::string href;
    std::string type;
};

// <entry>
struct Entry
{
    Entry(const std::string& upd, const std::string& id_,
          const std::string& title_, const std::vector<Link>& links_,
          const std::vector<Content>& content_,
          const std::vector<Author>& authors_);
    Entry() = default;
    std::string updated;
    std::string id;
    std::string title;
    std::vector<Link> links;
    std::vector<Content> content;
    std::vector<Author> authors;
};

// root element <feed>
struct Feed
{
    Feed(const std::string& updated_, const std::string& id_,
         const std::string& icon_, const std::string& title_);
    Feed() = default;
    std::string updated;
    std::string id;
    std::string icon;
    std::string title;
    std::vector<Link> links;
    std::vector<Entry> entries;
};

std::ostream& operator<<(std::ostream& os, const opds::Feed& feed);
std::ostream& operator<<(std::ostream& os, const opds::Link& link);
std::ostream& operator<<(std::ostream& os, const opds::Entry& entry);
std::ostream& operator<<(std::ostream& os, const opds::Content& content);
std::ostream& operator<<(std::ostream& os, const opds::Author& author);


}  // namespace application::utility::opds
