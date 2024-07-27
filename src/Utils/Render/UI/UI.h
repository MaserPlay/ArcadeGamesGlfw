//
// Created by super on 23.07.2024.
//

#ifndef ARCADEGAMES_UI_H
#define ARCADEGAMES_UI_H
#include "glm/mat4x4.hpp"
#include "MergedRender.h"

namespace UI {
    glm::mat4 Matrix(int width, int height);
    void DarkerBackground(MergedRender*);
    glm::vec2 vector_to_uicoords(double xpos, double ypos);
}

#endif //ARCADEGAMES_UI_H
