//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_DEBUG_H
#define ARCADEGAMESGLFW_DEBUG_H

#include "boxer/boxer.h"
#include <iostream>
#define Info(why) boxer::show(why, "Info", boxer::Style::Info);

#define Warning(why) boxer::show(why, "Warning!", boxer::Style::Warning);

#define Error(why) boxer::show(why, "Error", boxer::Style::Error);

#define ErrorThrowExs(why, exs) Error(why) throw exs(why);
#define ErrorThrow(why) ErrorThrowExs(why, std::exception)

#define ErrorAbort(why) Error(why) abort();

#define Print(text) std::cout << text << std::endl;

#endif //ARCADEGAMESGLFW_DEBUG_H
