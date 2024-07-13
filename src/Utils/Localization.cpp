//
// Created by super on 12.07.2024.
//

#include "Localization.h"
#include <fstream>
#include "nlohmann/json.hpp"
#include "map"
#include "SystemAdapter.h"

nlohmann::json local {};
std::string lang = "en";
#ifdef _DEBUG
bool write = false;
#endif

namespace Localization{
    void init(){
        lang = std::setlocale(0, "");
        std::setlocale(LC_NUMERIC, "C");
        std::ifstream f(SystemAdapter::GetGameFolderName("Localization")  + "lang.lang");
        if (f.is_open()) {
            try{
                local = nlohmann::json::parse(f);
            } catch (std::exception e)
            {
                SPDLOG_WARN(e.what());
            }
        } else {
            SPDLOG_WARN("cannot open SystemAdapter::GetGameFolderName(\"Localization\")  + \"lang.lang\"");
        }
    }
    std::string getTrans(std::string en){
#ifdef _DEBUG
        if (write) {
            local[en]["en"] = en;
        }
#endif
        if (local[en][lang].empty())
        {
            return en;
        } else {
            return local[en][lang];
        }
    }
    void setLang(std::string code){
        lang = code;
    }
#ifdef _DEBUG
    void genfile(){
// write prettified JSON to another file
        std::ofstream o(SystemAdapter::GetGameFolderName("Localization")  + "lang.lang");
        o << local;
    }
    void writefile(){
        write = true;
    }
#endif
}