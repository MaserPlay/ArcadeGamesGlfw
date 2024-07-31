//
// Created by super on 31.07.2024.
//

#ifndef ARCADEGAMES_SYSTEM_H
#define ARCADEGAMES_SYSTEM_H
#include "string"
#include "SystemAdapter.h"
#ifdef _WINDOWS
#include <windows.h>
#include <shellapi.h>

#endif

namespace SystemAdapter {
    inline
    std::string GetGameFolderName(std::string name){
        if (!name.empty()) {
            try {
                std::filesystem::create_directory(name);
            } catch (std::filesystem::filesystem_error e) {
                spdlog::error(e.what());
            }
            return std::filesystem::current_path().string() + "\\" + name + "\\";
        } else {
            return std::filesystem::current_path().string() + "\\";
        }
    }
}

#endif //ARCADEGAMES_SYSTEM_H
