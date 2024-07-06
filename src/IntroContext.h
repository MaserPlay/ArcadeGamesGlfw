//
// Created by super on 06.07.2024.
//

#ifndef ARCADEGAMES_INTROCONTEXT_H
#define ARCADEGAMES_INTROCONTEXT_H


#include <deque>
#include "Context.h"
//SOUND
#include "Sound.h"
//Snake
#include "Snake.h"

struct SnakeBodyUpdated : public glm::vec<2, short>{
    SnakeBodyUpdated(unsigned int xy, Directions direction) : glm::vec<2, short>(xy), direction(direction) {}

    SnakeBodyUpdated(unsigned int x, unsigned int y, Directions direction) : glm::vec<2, short>(x, y), direction(direction) {}

    Directions direction;
};

class IntroContext : public Context {
public:
    ~IntroContext() override;

    void init() override;

    void loop() override;

    void size_callback(int width, int height) override;

    void key_callback(int key, int scancode, int action, int mods) override;
private:
    void loadResources();
    void loadSnakeResources();
    void SkipIntro();
    glm::vec<2, int> screensize {8,8};
    //THREAD
    std::thread t;
    //SNAKES
    std::vector<std::deque<SnakeBodyUpdated>> Snakes {{}, {}, {}, {}};
    //TEMP
    float StartTime;
    //TEXTURES
    Texture* PresentsTexture = new Texture();
    Texture* LogoTexture = new Texture();

    //SNAKE
    float lastTickTime;
    //TEXTURES
    Texture* AngleTexture = new Texture();
    Texture* TailTexture = new Texture();
    Texture* HeadTexture = new Texture();
    Texture* BodyTexture = new Texture();
    //RENDER
    void renderTile(glm::vec<2, short> coords, Texture* t, glm::vec4 color);
    //TEXTURE COORDS
    const std::array<glm::vec2, 4> texturecordsUp = {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};
    const std::array<glm::vec2, 4> texturecordsLeft = {glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsDown = {glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsRight = {glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0)};
};


#endif //ARCADEGAMES_INTROCONTEXT_H
