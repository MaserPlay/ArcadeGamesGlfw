//
// Created by super on 17.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKE_H
#define ARCADEGAMESGLFW_SNAKE_H


#include <deque>
#include "BaseGame.h"
#include "Other.h"
#include "Texture.h"
#include "SnakeMap.h"
// OPENAL
#include <al.h>

struct SnakeBody : public Coords{
    SnakeBody(unsigned int xy, Directions direction) : Coords(xy), direction(direction) {}

    SnakeBody(unsigned int x, unsigned int y, Directions direction) : Coords(x, y), direction(direction) {}

    Directions direction;
};

class Snake : public BaseGame {
public:
    ~Snake() override;

    void init() override;
    void loop() override;
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
private:
    void loadResources();
    bool CheckCollision(Coords c, Coords s);
    void MoveSnake(SnakeBody to);
    void renderTile(Coords coords, std::array<glm::vec2, 4> v = {});
    void Reset();
    void ResetApple(unsigned short counter = 306);
    std::deque<SnakeBody> snake {};
    Directions direction = Directions::Down;
    Coords Apple = {5,5};
    SnakeMap* map;
    // TEMP
    float lastTickTime;
    //TICKS
    float tickSpeed = .4; // s
    // TEXTURES
    Texture* AngleTexture = new Texture();
    Texture* TailTexture = new Texture();
    Texture* AppleTexture = new Texture();
    Texture* HeadTexture = new Texture();
    Texture* BodyTexture = new Texture();
    Texture* WallTexture = new Texture();
    //AUDIO
    ALuint Eat = 0;
    ALuint source;
};


#endif //ARCADEGAMESGLFW_SNAKE_H
