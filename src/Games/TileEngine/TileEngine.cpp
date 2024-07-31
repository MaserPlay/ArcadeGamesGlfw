//
// Created by super on 04.07.2024.
//

#include <zipconf.h>
#include <stb_image.h>
#include "TileEngine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

bool TileEngine::CheckCollision(Coords<> c, Coords<> s) {
    return c == s;
}

void TileEngine::size_callback(int width, int height, const Coords<> screensize) {
    glViewport(0,0, width, height);
    if (width >= height) {
        float Shift = (((float) width / (float) height) - ((float) screensize.x / (float) screensize.y)) * ((float) screensize.x / 2.f) - 1.f;
        projectionMatrix = glm::ortho( -Shift, (float) screensize.x * ((float) width / (float) height) - Shift, 0.f + 1.f, (float) screensize.y + 1);
    } else {
        float Shift = (((float) height / (float) width) - ((float) screensize.y / (float) screensize.x)) * ((float) screensize.y / 2.f) - 1.f;
        projectionMatrix = glm::ortho(0.f + 1.f, (float) screensize.x + 1.f, -Shift, (float) screensize.y * ((float) height / (float) width) - Shift);
    }
}

void TileEngine::use(MergedRender* render) {
    render->use(projectionMatrix);
}

glm::vec2 TileEngine::vector_to_screencoords(double xpos, double ypos, const Coords<> screensize) {
    if (getWidth() >= getHeight()) {
        auto Shift = (((float) getWidth() / (float) getHeight()) - ((float) screensize.x / (float) screensize.y)) * (float) (screensize.x / 2);
        return {(((xpos / getWidth())) * (screensize.x + Shift * 2) + 1) - Shift, (1 - (ypos / getHeight())) * screensize.y + 1};
    } else {
        auto Shift = (((float) getHeight() / (float) getWidth()) - ((float) screensize.y / (float) screensize.x)) * (float) (screensize.y / 2);
        return { ( (xpos / getWidth())) * screensize.x + 1,((1 - (ypos / getHeight())) * (screensize.y + Shift * 2) + 1) - Shift};
    }
}
