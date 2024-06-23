//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_PACMAN_H
#define ARCADEGAMESGLFW_PACMAN_H


#include <glm/vec2.hpp>
#include "Other.h"
#include "BaseGame.h"
#include "array"

class Pacman : public BaseGame {
public:
    void init() override;

    void loop() override;

    void size_callback(int width, int height) override;

    void key_callback(int key, int scancode, int action, int mods) override;

private:
    bool CheckCollision(Coords c, Coords s);
    void renderTile(Coords coords, std::array<glm::vec2, 4> v);

};


#endif //ARCADEGAMESGLFW_PACMAN_H
