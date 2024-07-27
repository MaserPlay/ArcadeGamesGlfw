//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_FILEOPENDIALOG_H
#define ARCADEGAMESGLFW_FILEOPENDIALOG_H

#include <vector>
#include "nfd.h"
#include "string"
#include "filesystem"
#include "debug.h"
#include <fstream>

namespace SystemAdapter {
    void Init();

    typedef std::vector<nfdfilteritem_t> FileFilter;

    std::string OpenFileDialog(const FileFilter& filter, const std::string& defaultpath = "");
    std::string GetGameFolderName(std::string name);
    std::string ReadAll(std::ifstream &f);
    void OpenLink(std::string);

    void Destroy();
}

#endif //ARCADEGAMESGLFW_FILEOPENDIALOG_H
