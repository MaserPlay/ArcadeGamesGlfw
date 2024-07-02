//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_DEBUG_H
#define ARCADEGAMESGLFW_DEBUG_H

#include "boxer/boxer.h"
#include <iostream>
#include "spdlog/spdlog.h"
#define Info(why) boxer::show(why, "Info", boxer::Style::Info);

//#define Warning(why) boxer::show(why, "Warning!", boxer::Style::Warning);
#ifdef _DEBUG
#define Error(why) boxer::show(why, "Error", boxer::Style::Error);
#define Print(text) std::cout << text << std::endl;
#else
#define Error(why)
#define Print(text)
#endif

#define ErrorThrowExs(why, exs) spdlog::error(why); throw exs(why);
#define ErrorThrow(why) ErrorThrowExs(why, std::exception)

#define ErrorAbort(why) spdlog::error(why); abort();

#endif //ARCADEGAMESGLFW_DEBUG_H
