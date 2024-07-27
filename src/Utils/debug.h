//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_DEBUG_H
#define ARCADEGAMESGLFW_DEBUG_H

#include "boxer/boxer.h"
#include <iostream>
#include "LOG.h"
#define Info(why) boxer::show(why, "Info", boxer::Style::Info);

//#define Warning(why) boxer::show(why, "Warning!", boxer::Style::Warning);
#ifdef _DEBUG
#define ErrorBox(why) boxer::show(why, "ErrorBox", boxer::Style::Error);
#define Print(text) std::cout << text << std::endl;
#else
#define ErrorBox(why)
#define Print(text)
#endif

#define ErrorThrowExs(why, exs) SPDLOG_CRITICAL(why); throw exs(why);
#define ErrorThrow(why) ErrorThrowExs(why, std::exception)

#define ErrorAbort(why) SPDLOG_CRITICAL(why); abort();

#endif //ARCADEGAMESGLFW_DEBUG_H
