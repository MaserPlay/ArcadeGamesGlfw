//
// Created by super on 17.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKE_H
#define ARCADEGAMESGLFW_SNAKE_H


#include <deque>
#include "SnakeMap.h"
#include "TileEngine.h"
#include "Button.h"



class Snake : public TileEngine {
private:
    struct SnakeBody : public Coords{
        SnakeBody(unsigned int xy, Directions direction) : Coords(xy), direction(direction) {}

        SnakeBody(unsigned int x, unsigned int y, Directions direction) : Coords(x, y), direction(direction) {}

        Directions direction {};
        MergedRender* render = new MergedRender();
    };
public:
    ~Snake() override;

    void init() override;
    void loop() override;
    void size_callback(int width, int height) override;
    void key_callback(int key, int scancode, int action, int mods) override;
    void cursor_position_callback(double xpos, double ypos) override {};

    void mouse_button_callback(int button, int action, int mods) override;

private:
    void useCoordsAndTextureAndLoad(SnakeBody&, Texture*);
    void loopPause();
    void Server();
    void loadResources();
    void MoveSnake(SnakeBody to);
    void Reset();
    void ResetApple();
    void SelectAndOpenMap();
    bool OpenMap(std::string path);
    std::deque<SnakeBody> snake {};
    Directions direction = Down;
    Coords AppleCoords;
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
    Texture* HeadTexture = new Texture();
    Texture* BodyTexture = new Texture();
    Texture* WallTexture = new Texture();
    //TEXTURE COORDS
    const std::array<glm::vec2, 4> texturecordsUp = {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};
    const std::array<glm::vec2, 4> texturecordsLeft = {glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsDown = {glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsRight = {glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0)};
    // RENDER
    MergedRender* Grid {new MergedRender()};
    MergedRender* Apple {new MergedRender()};
    std::vector<MergedRender*> WallsRender {};
    std::vector<std::unique_ptr<MergedRender>> ScoreBuffer = {};
    //UI
    std::vector<Button> pauseButtons {};
};


#endif //ARCADEGAMESGLFW_SNAKE_H
