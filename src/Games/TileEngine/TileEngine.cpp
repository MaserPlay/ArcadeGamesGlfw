//
// Created by super on 04.07.2024.
//

#include "TileEngine.h"

bool TileEngine::CheckCollision(Coords c, Coords s) {
    return c == s;
}

void TileEngine::size_callback(int width, int height, const glm::vec<2, unsigned int> screensize) {
    glViewport(0,0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    auto& screensize = map->getField();
    double Shift = 0;
    if (width >= height) {
        Shift = (((float) width / (float) height) - ((float) screensize.x / (float) screensize.y)) * (float) (screensize.x / 2) - 1.;
        glOrtho( -Shift, (float) screensize.x * ((float) width / (float) height) - Shift, 0. + 1, (float) screensize.y + 1, -2., 2.);
    } else {
        Shift = (((float) height / (float) width) - ((float) screensize.y / (float) screensize.x)) * (float) (screensize.y / 2) - 1.;
        glOrtho(0. + 1, (float) screensize.x + 1, -Shift, (float) screensize.y * ((float) height / (float) width) - Shift, -2., 2.);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

glm::vec2 TileEngine::vector_to_screencoords(double xpos, double ypos, const glm::vec<2, unsigned int> screensize) {
    if (getWidth() >= getHeight()) {
        auto Shift = (((float) getWidth() / (float) getHeight()) - ((float) screensize.x / (float) screensize.y)) * (float) (screensize.x / 2);
//        glOrtho( -Shift, (float) screensize.x * ((float) width / (float) height) - Shift, 0. + 1, (float) screensize.y + 1, -2., 2.);
        return {(((xpos / getWidth())) * (screensize.x + Shift * 2) + 1) - Shift, (1 - (ypos / getHeight())) * screensize.y + 1};
    } else {
        auto Shift = (((float) getHeight() / (float) getWidth()) - ((float) screensize.y / (float) screensize.x)) * (float) (screensize.y / 2);
//        glOrtho(0. + 1, (float) screensize.x + 1, -Shift, (float) screensize.y * ((float) height / (float) width) - Shift, -2., 2.);
        return { ( (xpos / getWidth())) * screensize.x + 1,((1 - (ypos / getHeight())) * (screensize.y + Shift * 2) + 1) - Shift};
    }
}

void TileEngine::initEngine() {
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_ALPHA_TEST);
//    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
}

void TileEngine::renderTile(Coords coords, Texture *t, glm::vec4 c, double z) {
    if (t->getInitImage() == 0 ) {glColor4d(c.r,c.g,c.b, c.a);} else {glColor4d(1.,1.,1., 1.);}
    glBindTexture(GL_TEXTURE_2D, t->getInitImage());
    glBegin(GL_QUADS);
    glTexCoord2d(t->texturecords[0].x, t->texturecords[0].y);
    glVertex3d(coords.x + 1,coords.y, z);
    glTexCoord2d(t->texturecords[1].x, t->texturecords[1].y);
    glVertex3d(coords.x + 1,coords.y + 1, z);
    glTexCoord2d(t->texturecords[2].x, t->texturecords[2].y);
    glVertex3d(coords.x,coords.y + 1, z);
    glTexCoord2d(t->texturecords[3].x, t->texturecords[3].y);
    glVertex3d(coords.x,coords.y, z);
    glEnd();
}