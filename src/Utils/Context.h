//
// Created by super on 20.06.2024.
//

#ifndef ARCADEGAMESGLFW_CONTEXT_H
#define ARCADEGAMESGLFW_CONTEXT_H

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

class Context {
public:
    Context() = default;
    Context(const Context &p) = delete;   // удаляем конструктор
    virtual void init() = 0;
    virtual void loop() = 0;

    virtual void size_callback(int width, int height) = 0;
    virtual void key_callback(int key, int scancode, int action, int mods) = 0;
    virtual void mouse_button_callback(int button, int action, int mods) = 0;
    virtual void cursor_position_callback(double xpos, double ypos) = 0;

    virtual ~Context() = default;
};

//MAIN
#include "main.h"


#endif //ARCADEGAMESGLFW_CONTEXT_H
