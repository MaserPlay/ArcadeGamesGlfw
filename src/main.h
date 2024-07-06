//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_MAIN_H
#define ARCADEGAMESGLFW_MAIN_H

#include "Context.h"
#include "BaseGame.h"
#include "string"
#include "Font.h"

Font::Font* getFont();
GLFWwindow* getwindow();
void setContext(Context* context);
void loadMainMenu();
GLuint getWidth();
GLuint getHeight();
void SetIcon(const std::string& name);
void InitAsync();

#endif //ARCADEGAMESGLFW_MAIN_H
