//
// Created by super on 05.07.2024.
//

#ifndef ARCADEGAMES_LOG_H
#define ARCADEGAMES_LOG_H

#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#endif
#include "spdlog/spdlog.h"

#endif //ARCADEGAMES_LOG_H
