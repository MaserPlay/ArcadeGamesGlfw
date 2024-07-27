//
// Created by super on 12.07.2024.
//

#ifndef ARCADEGAMES_LOCALIZATION_H
#define ARCADEGAMES_LOCALIZATION_H

#include <string>
#include "LOG.h"

namespace Localization{
    void init();
    std::string getTrans(std::string en);
    void setLang(std::string code);
#define _(e) Localization::getTrans(e)
#define _c(e) Localization::getTrans(e).c_str()
#ifdef _DEBUG
    void genfile();
    void writefile();
#endif
}

#endif //ARCADEGAMES_LOCALIZATION_H
