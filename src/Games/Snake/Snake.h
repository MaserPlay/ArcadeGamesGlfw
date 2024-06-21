//
// Created by super on 17.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKE_H
#define ARCADEGAMESGLFW_SNAKE_H


#include <deque>
#include "BaseGame.h"
#include "main.h"
#include "Other.h"
#include "Texture.h"

struct SnakeBody : public Coords{
    SnakeBody(unsigned int x, unsigned int y, Directions direction) : Coords(x, y), direction(direction) {}

    Directions direction;
};

class Snake : public BaseGame {
public:
    ~Snake() override;

    void init() override;
    void loadResources();
    bool CheckCollision(Coords c, Coords s);
    void MoveSnake(SnakeBody to);
    void loop() override;
    void renderTile(Coords coords, std::array<glm::vec2, 4> v);
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
    void Reset();
    void ResetApple();
    bool isCrashed = false;
    std::deque<SnakeBody> snake {};
    Directions direction = Directions::Right;
    Coords Apple = {5,5};
    // TEXTURES
    Texture* AppleTexture = new Texture();
    Texture* HeadTexture = new Texture();
    Texture* BodyTexture = new Texture();
    Texture* AngleTexture = new Texture();
    Texture* TailTexture = new Texture();
};


#endif //ARCADEGAMESGLFW_SNAKE_H
