//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMESGLFW_MAIN_H
#define ARCADEGAMESGLFW_MAIN_H

#include "Context.h"
#include "Utils/BaseGame.h"
#include "string"
#include "Utils/Render/Font.h"

Font::Font* getFont();
GLFWwindow* getwindow();
#ifdef _DEBUG
bool getFps();
#endif
void SwitchFullscreen();
void setContext(Context* context);
void loadMainMenu();
GLuint getWidth();
GLuint getHeight();
void SetIcon(const std::string& name);
void Init();

#endif //ARCADEGAMESGLFW_MAIN_H
