//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_MAIN_H
#define ARCADEGAMESGLFW_MAIN_H

#include "Context.h"
#include "BaseGame.h"
#include "string"

GLFWwindow* getwindow();
void setContext(Context* context);
void loadMainMenu();
GLuint getWidth();
GLuint getHeight();
void SetIcon(const std::string& name);

#endif //ARCADEGAMESGLFW_MAIN_H
