//
// Created by super on 24.06.2024.
//

#include "ZipArchive.h"
#include "SystemAdapter.h"

ZipArchive::ZipArchive(const std::string& name) {
    int *err = NULL;
    archive = zip_open((name).c_str(), ZIP_RDONLY, err);
    if (archive == NULL)
    {
        zip_error_t error;
        zip_error_init_with_code(&error, *err);
        spdlog::error((std::string("cannot open zip archive") + zip_error_strerror(&error)).c_str());
        zip_error_fini(&error);
        return;
    }
}

struct zip_stat ZipArchive::getStat(const std::string& name) {
    //Search for the file of given name
    struct zip_stat st;
    zip_stat_init(&st);
    zip_stat(archive, name.c_str(), 0, &st);
    return st;
}

char *ZipArchive::getContent(const std::string& name) {
    return getContent(name, getStat(name));
}

ZipArchive::~ZipArchive() {
    zip_close(archive);
    //delete archive;
}

char *ZipArchive::getContent(const std::string& name, struct zip_stat stat) {
    char *contents = new char[stat.size];

    //Read the compressed file
    zip_file *f = zip_fopen(archive, name.c_str(), 0);
    zip_fread(f, contents, stat.size);
    zip_fclose(f);
    return contents;
}

void ZipArchive::get(const std::string& name, char*& content, zip_uint64_t& size) {
    auto stat = getStat(name);
    if (stat.size <= 0)
    {
        content = NULL;
        size = 0;
    } else {
        size = stat.size;
        content = getContent(name, stat);
    }
}
