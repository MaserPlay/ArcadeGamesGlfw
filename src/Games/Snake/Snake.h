//
// Created by super on 17.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKE_H
#define ARCADEGAMESGLFW_SNAKE_H


#include <deque>
#include "SnakeMap.h"
#include "TileEngine.h"

struct SnakeBody : public Coords{
    SnakeBody(unsigned int xy, Directions direction) : Coords(xy), direction(direction) {}

    SnakeBody(unsigned int x, unsigned int y, Directions direction) : Coords(x, y), direction(direction) {}

    Directions direction;
};

class Snake : public TileEngine {
public:
    ~Snake() override;

    void init() override;
    void loop() override;
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
    void cursor_position_callback(double xpos, double ypos) override {};

    void mouse_button_callback(int button, int action, int mods) override;

private:
    void loopPause();
    void Server();
    void loadResources();
    void MoveSnake(SnakeBody to);
    void Reset();
    void ResetApple();
    std::deque<SnakeBody> snake {};
    Directions direction = Down;
    Coords Apple;
    SnakeMap* map;
    // TEMP
    bool Pause = false;
    float lastTickTime;
    int Score = 0;
    //AUDIO
    Sound* Eat = new Sound();
    // TEXTURES
    Texture* AngleTexture = new Texture();
    Texture* TailTexture = new Texture();
    Texture* AppleTexture = new Texture();
    Texture* HeadTexture = new Texture();
    Texture* BodyTexture = new Texture();
    Texture* WallTexture = new Texture();
    //TEXTURE COORDS
    const std::array<glm::vec2, 4> texturecordsUp = {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};
    const std::array<glm::vec2, 4> texturecordsLeft = {glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsDown = {glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsRight = {glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0)};
};


#endif //ARCADEGAMESGLFW_SNAKE_H
