#pragma once
#include <QDir>
#include <QString>

namespace application::utility::opds
{


/**
 * @brief OpdsService::unzipFile find file with specific format in zip  file
 * ,unpack  and delete zip archive (recursive)
 * @param src Full path to source zip archive
 * @param format - For example - pdf.zip or epub.zip -- <search-for-format>.zip
 * @param dest_folder - QDir - folder to save unzipped file
 * @param dest_file_name - filename of unpacked file without path and extension
 * @return Qstring  - full path to unpacked file or empty string if not
 * succeeded
 */
QString unzipFile(const QString& src, const QString& format,
                  const QDir& dest_folder, const QString dest_file_name);

}  // namespace application::utility::opds
