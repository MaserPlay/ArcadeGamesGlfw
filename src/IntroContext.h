//
// Created by super on 06.07.2024.
//

#ifndef ARCADEGAMES_INTROCONTEXT_H
#define ARCADEGAMES_INTROCONTEXT_H


#include <deque>
#include "Context.h"
//SOUND
#include "Audio/Sound.h"
//Snake
#include "Snake.h"

typedef Coords<short> IntroCoords;


class IntroContext : public TileEngine {
private:
    using SnakeBodyUpdated = SnakeBody<short>;
public:
    ~IntroContext() override;

    void init() override;

    void loop() override;

    void size_callback(int width, int height) override;

    void key_callback(int key, int scancode, int action, int mods) override;
    void cursor_position_callback(double xpos, double ypos) override {}

    void mouse_button_callback(int button, int action, int mods) override {}

private:
    void useCoordsAndTextureAndLoad(SnakeBodyUpdated&, const std::shared_ptr<Texture>&);
    void MoveSnake(SnakeBodyUpdated to, std::deque<SnakeBodyUpdated>& snake);
    void Server();
    void loadResources();
    void loadSnakeResources();
    void SkipIntro();
    Coords<int> screensize {8,8};
    //SNAKES
    std::vector<std::deque<SnakeBodyUpdated>> Snakes {};
    //TEMP
    float StartTime;

    //SNAKE
    float lastTickTime;
    //TEXTURES
    std::shared_ptr<Texture> AngleTexture {new Texture()};
    std::shared_ptr<Texture> TailTexture {new Texture()};
    std::shared_ptr<Texture> HeadTexture {new Texture()};
    std::shared_ptr<Texture> BodyTexture {new Texture()};
    //TEXTURE COORDS
    const std::array<glm::vec2, 4> texturecordsUp = {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};
    const std::array<glm::vec2, 4> texturecordsLeft = {glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsDown = {glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsRight = {glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0)};
    //UI
    std::vector<std::unique_ptr<MergedRender>> PresentsBuffer = {};
    // RENDER
    std::unique_ptr<MergedRender> Grid {new MergedRender()};
    std::unique_ptr<MergedRender> LogoRender {new MergedRender()};
};


#endif //ARCADEGAMES_INTROCONTEXT_H
