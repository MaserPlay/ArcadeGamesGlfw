//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_TILEENGINE_H
#define ARCADEGAMES_TILEENGINE_H


#include "BaseGame.h"
#include "Other.h"
#include "Texture.h"
#include "Sound.h"
#include "glm/vec4.hpp"

class TileEngine : public BaseGame {
protected:
    bool CheckCollision(Coords c, Coords s);
    void size_callback(int width, int height, const glm::vec<2, unsigned int> screensize);
    void initEngine();
    void renderTile(Coords coords, Texture* t, glm::vec4 color);
};


#endif //ARCADEGAMES_TILEENGINE_H
