//
// Created by super on 22.06.2024.
//

#ifndef ARCADEGAMESGLFW_PACMAN_H
#define ARCADEGAMESGLFW_PACMAN_H

#include "Utils/BaseGame.h"
#include "Other.h"
#include "Texture.h"
#include "TileEngine.h"
// OPENAL
#include <al.h>

class PacmanMap;

class Pacman : public TileEngine {
public:
    ~Pacman() override;

    void init() override;
    void loop() override;
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
    void cursor_position_callback(double xpos, double ypos) override;

    void mouse_button_callback(int button, int action, int mods) override;

private:
    void loadResources();
    bool CheckCollision(Coords<> c);
    void Reset();
    Coords<> pacmanPos;
    std::vector<Coords<>> MoventGrid {};
    Directions direction = Directions::Left;
    PacmanMap* map;
    // TEMP
    float lastTickTime;
    //TICKS
    float tickSpeed = .4; // s
};


#endif //ARCADEGAMESGLFW_PACMAN_H
