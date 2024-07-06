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
    if ((float) width >= (float) height) {
        Shift = (((float) width / (float) height) - ((float) screensize.x / (float) screensize.y)) * 5 - 1.;
        glOrtho( -Shift, (float) screensize.x * ((float) width / (float) height) - Shift, 0. + 1, (float) screensize.y + 1, 0., 1.);
    } else {
        Shift = (((float) height / (float) width) - ((float) screensize.y / (float) screensize.x)) * 5 - 1.;
        glOrtho(0. + 1, (float) screensize.x + 1, -Shift, (float) screensize.y * ((float) height / (float) width) - Shift, 0., 1.);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void TileEngine::initEngine() {
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_ALPHA_TEST);
//    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
}

void TileEngine::renderTile(Coords coords, Texture *t, glm::vec4 c) {
    if (t->getInitImage() == 0 ) {glColor4d(c.r,c.g,c.b, c.a);} else {glColor4d(1.,1.,1., 1.);}
    glBindTexture(GL_TEXTURE_2D, t->getInitImage());
    glBegin(GL_QUADS);
    glTexCoord2d(t->texturecords[0].x, t->texturecords[0].y);
    glVertex2d(coords.x + 1,coords.y);
    glTexCoord2d(t->texturecords[1].x, t->texturecords[1].y);
    glVertex2d(coords.x + 1,coords.y + 1);
    glTexCoord2d(t->texturecords[2].x, t->texturecords[2].y);
    glVertex2d(coords.x,coords.y + 1);
    glTexCoord2d(t->texturecords[3].x, t->texturecords[3].y);
    glVertex2d(coords.x,coords.y);
    glEnd();
}
