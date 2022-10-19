#include "downloaded_books_tracker.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <iterator>
#include <QDebug>
#include <QFile>


using namespace domain::models;


namespace application::utility
{

void DownloadedBooksTracker::setLibraryOwner(const QString& libraryOwnerEmail)
{
    m_libraryOwnerEmail = libraryOwnerEmail;
    m_libraryFolder = getUserLibraryDir();
}


std::vector<Book> DownloadedBooksTracker::getTrackedBooks()
{
    ensureUserLibraryExists();
    
    QDir libraryDir = getUserLibraryDir();
    
    std::vector<Book> books;
    for(auto& metaFileName : libraryDir.entryList(QDir::Files))
    {
        QFile metaFile(libraryDir.path() + "/" + metaFileName);
        if(!metaFile.open(QFile::ReadOnly))
            return {};
        
        auto jsonDoc = QJsonDocument::fromJson(metaFile.readAll());
        auto bookObject = jsonDoc.object();
        
        books.emplace_back(Book::fromJson(bookObject));
    }
    
    return books;
}

bool DownloadedBooksTracker::trackBook(const Book& book)
{
    ensureUserLibraryExists();
    
    QDir libraryDir = getUserLibraryDir();
    QFile file(libraryDir.path() + "/" + book.getUuid()
               .toString(QUuid::WithoutBraces) + m_fileExtension);
    
    if(!file.open(QFile::WriteOnly))
        return false;
    
    file.write(book.serializeToJson());
    return true;
}

bool DownloadedBooksTracker::untrackBook(const QUuid& uuid)
{
    ensureUserLibraryExists();
    
    QDir libraryDir = getUserLibraryDir();
    QString fileToUntrack = uuid.toString(QUuid::WithoutBraces) + m_fileExtension;
    
    auto success = libraryDir.remove(fileToUntrack);
    
    return success;
}

bool DownloadedBooksTracker::updateTrackedBook(const Book& book)
{
    ensureUserLibraryExists();
    
    untrackBook(book.getUuid());
    trackBook(book);
    
    return true;
}


void DownloadedBooksTracker::ensureUserLibraryExists()
{
    auto libraryDir = getUserLibraryDir();
    
    libraryDir.mkpath(libraryDir.path());
}

QDir DownloadedBooksTracker::getUserLibraryDir() const
{
    auto applicationPath = QDir::current().path();
    auto userLibName = getUserLibraryName(m_libraryOwnerEmail);
    
    auto folderName = applicationPath + "/" + "librum_localLibraries" + "/"  + userLibName;
    return QDir(folderName);
}

QString DownloadedBooksTracker::getUserLibraryName(QString email) const
{
    auto it = std::remove_if(email.begin(), email.end(), [](const QChar& c) { 
        return !c.isLetterOrNumber();
    });
    
    int specialCharCount = std::distance(it, email.end());
    email.chop(specialCharCount);
    
    return email + QString(email.length(), '_');
}

} // namespace application::utility