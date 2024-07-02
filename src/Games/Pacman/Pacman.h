//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_PACMAN_H
#define ARCADEGAMESGLFW_PACMAN_H

#include "BaseGame.h"
#include "Other.h"
#include "Texture.h"
// OPENAL
#include <al.h>

class PacmanMap;

class Pacman : public BaseGame {
public:
    ~Pacman() override;

    void init() override;
    void loop() override;
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
private:
    void loadResources();
    bool CheckCollision(Coords c, Coords s);
    void renderTile(Coords coords, std::array<glm::vec2, 4> v = {});
    void Reset();
    Directions direction = Directions::Down;
    PacmanMap* map;
    // TEMP
    float lastTickTime;
    //TICKS
    float tickSpeed = .4; // s
};


#endif //ARCADEGAMESGLFW_PACMAN_H
