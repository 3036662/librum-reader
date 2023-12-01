#include "opds_structs.hpp"

namespace application::utility::opds
{
Feed::Feed(const std::string& updated_, const std::string& id_,
           const std::string& icon_ = "", const std::string& title_ = "Root")
    : updated(updated_),
      id(id_),
      icon(icon_),
      title(title_)
{
}

Link::Link(const std::string& rel_, const std::string& href_,
           const std::string& type_)
    : rel(rel_),
      href(href_),
      type(type_)
{
}

Content::Content(const std::string& type_, const std::string& text_)
    : type(type_),
      text(text_)
{
}

Author::Author(const std::string& name_, const std::string& uri_)
    : name(name_),
      uri(uri_)
{
}

Entry::Entry(const std::string& upd, const std::string& id_,
             const std::string& title_, const std::vector<Link>& links_,
             const std::vector<Content>& content_,
             const std::vector<Author>& authors_)
    : updated(upd),
      id(id_),
      title(title_),
      links(links_),
      content(content_),
      authors(authors_)
{
}

std::ostream& operator<<(std::ostream& os, const opds::Feed& feed)
{
    os << std::setw(130) << std::setfill('#') << '#' << std::endl;
    os << std::setfill(' ') << std::setw(20) << "Root Elemet:" << std::setw(20)
       << feed.title << std::setw(20) << "ID: " << std::setw(20) << feed.id
       << std::endl;
    os << std::setw(130) << std::setfill('-') << '-' << std::endl;
    // entries
    for(auto it = feed.entries.begin(); it != feed.entries.end(); ++it)
    {
        os << *it;
        os << std::setw(130) << std::setfill('.') << '.' << std::endl;
    }
    os << std::setw(130) << std::setfill('-') << '-' << std::endl;
    //  Link
    os << "Links:\n";
    for(auto it = feed.links.begin(); it != feed.links.end(); ++it)
    {
        os << "\t" << *it;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const opds::Link& link)
{
    // os<< link.rel << "\t(Link rel)\thref: \t" << link.href.substr(0,30) <<
    // "...." << "\t type: \t" << link.type << std::endl;
    os << link.rel << "\t(Link rel)\thref: \t" << link.href << "\t type: \t"
       << link.type << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const opds::Entry& entry)
{
    os << entry.title << "\t(Entry title)\tid:\t" << entry.id << std::endl;
    os << "\t\t"
       << "Entry links:\n";
    for(auto it = entry.links.begin(); it != entry.links.end(); ++it)
    {
        os << "\t\t\t" << *it;
    }
    os << "\t\t"
       << "Entry content:\n";
    for(auto it = entry.content.begin(); it != entry.content.end(); ++it)
    {
        os << "\t\t\t" << *it;
    }
    os << "\t\t"
       << "Entry Authors:\n";
    for(auto it = entry.authors.begin(); it != entry.authors.end(); ++it)
    {
        os << "\t\t\t" << *it;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const opds::Content& content)
{
    os << "Content:\t" << content.text << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const opds::Author& author)
{
    os << "Author:\t" << author.name << "\turl:\t" << author.uri << std::endl;
    return os;
}


}  // namespace application::utility::opds
