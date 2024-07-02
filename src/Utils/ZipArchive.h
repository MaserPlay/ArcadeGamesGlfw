//
// Created by super on 24.06.2024.
//

#ifndef ARCADEGAMES_ZIPARCHIVE_H
#define ARCADEGAMES_ZIPARCHIVE_H

#include "string"
//ZIP
#include "zip.h"

class ZipArchive {
public:
    explicit ZipArchive(const std::string& name);
    struct zip_stat getStat(const std::string& name);
    char *getContent(const std::string& name, struct zip_stat stat);
    char *getContent(const std::string& name);
    void get(const std::string& name, char*& content, zip_uint64_t& size);

    virtual ~ZipArchive();

private:
    zip *archive;
};


#endif //ARCADEGAMES_ZIPARCHIVE_H
