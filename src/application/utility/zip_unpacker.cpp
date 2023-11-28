#include "zip_unpacker.hpp"
#include <zip.h>

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
                  const QDir& dest_folder, const QString dest_file_name)
{
    QString res;
    const QString zip_format(".zip");

    // if src is not zip archive -return empty string;
    if(!format.endsWith(zip_format))
    {
        return res;
    }

    // get target format from format param
    QString target_format = format.chopped(zip_format.length());

    // open zip
    int error = 0;
    zip_t* zip_struct_pointer =
        zip_open(src.toStdString().c_str(), ZIP_RDONLY, &error);
    if(!zip_struct_pointer)
    {
        zip_error_t zError;
        zip_error_init_with_code(&zError, error);
        qDebug() << "Error opening zip " << zip_error_strerror(&zError);
        zip_error_fini(&zError);
        return res;
    }

    // check number of files
    zip_int64_t numberOfFiles = zip_get_num_entries(zip_struct_pointer, 0);
    if(numberOfFiles <= 0)
    {
        qDebug() << "Empty archive";
        zip_close(zip_struct_pointer);
        return res;
    }

    // iterate through files to find file of target format or nested archive
    zip_int64_t target_file_index = -1;
    zip_int64_t nested_archive_index = -1;
    for(zip_int64_t i = 0; i < numberOfFiles; ++i)
    {
        const char* name =
            zip_get_name(zip_struct_pointer, i, ZIP_FL_ENC_GUESS);
        if(!name)
        {
            qDebug() << "Cant read filename in archive";
            zip_close(zip_struct_pointer);
            return res;
        }
        if(QString(name).endsWith(target_format))
        {
            target_file_index = i;
            break;
        }
        if(QString(name).endsWith(zip_format))
        {
            nested_archive_index = i;
        }
    }

    // if nothing found
    if(target_file_index < 0 && nested_archive_index < 0)
    {
        qDebug() << "Cant find file of target format in archive";
        zip_close(zip_struct_pointer);
        return res;
    }

    // if no target file found, but nested atchive found - unpack nested
    bool unpack_nested { false };
    if(target_file_index < 0 && nested_archive_index >= 0)
    {
        target_file_index = nested_archive_index;
        target_format = format;
        unpack_nested = true;
    }

    // Unpack file
    zip_file_t* zipFile =
        zip_fopen_index(zip_struct_pointer, target_file_index, 0);
    if(!zipFile)
    {
        qDebug() << "Cant read file in archive";
        zip_close(zip_struct_pointer);
        return res;
    }
    auto unzipped_destination_filename = dest_file_name + "." + target_format;
    if(unpack_nested)
        unzipped_destination_filename += ".nested";
    struct zip_stat stat;
    zip_stat_init(&stat);  // get file size
    zip_stat_index(zip_struct_pointer, target_file_index, 0, &stat);
    std::shared_ptr<char[]> buff =
        std::make_unique<char[]>(stat.size);  // memory alloc
    zip_int64_t bytes_read = 0;
    if((bytes_read = zip_fread(zipFile, buff.get(), stat.size)) ==
       static_cast<zip_int64_t>(stat.size))
    {
        auto new_file =
            QFile(dest_folder.filePath(unzipped_destination_filename));
        if(new_file.open(QIODeviceBase::WriteOnly))
        {
            if(new_file.write(buff.get(), bytes_read) !=
               static_cast<qint64>(bytes_read))
            {
                qDebug() << "error writing to file while unzipping";
            }
        }
        new_file.close();
        res = dest_folder.filePath(unzipped_destination_filename);
        QFile old(src);
        // remove old file
        if(src != res && !old.remove())
        {
            qDebug() << "can''t delete file" << src;
        }
    }
    // free resources
    buff.reset();
    zip_fclose(zipFile);
    zip_close(zip_struct_pointer);
    // unpack from nested archive
    if(unpack_nested)
    {
        // we have .pdf.zip.nested
        QString nested_archive_filename =
            dest_folder.filePath(unzipped_destination_filename);
        res = unzipFile(nested_archive_filename, format, dest_folder,
                        dest_file_name);
    }

    // res= dest_file_name+target_format;
    return res;
}


}  // namespace application::utility::opds
