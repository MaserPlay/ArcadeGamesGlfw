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
    virtual void init() = 0;
    virtual void loop() = 0;

    virtual void size_callback(int width, int height) = 0;
    virtual void key_callback(int key, int scancode, int action, int mods) = 0;

    virtual ~Context() = default;
};

//MAIN
#include "main.h"


#endif //ARCADEGAMESGLFW_CONTEXT_H
