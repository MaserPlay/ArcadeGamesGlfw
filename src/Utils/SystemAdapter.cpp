//
// Created by super on 22.06.2024.
//

#include "SystemAdapter.h"

namespace SystemAdapter{
    void Init(){
        NFD_Init();
    }
    std::string OpenFileDialog(const FileFilter& filter, const std::string& defaultpath){
        nfdchar_t *outPath = NULL;
        const nfdu8char_t* input = NULL;
        if (defaultpath.empty())
        {
            input = NULL;
        } else {
            input = defaultpath.c_str();
        }
        nfdresult_t result = NFD_OpenDialog(&outPath, &filter[0], filter.size(), input);
        if (result == NFD_OKAY)
        {
            return std::string(outPath);
            NFD_FreePath(outPath);
        }
        return "";
    }
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
    void GetDefaultFont(){

    }

    void Destroy(){
        NFD_Quit();
    }
}