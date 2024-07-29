//
// Created by super on 17.06.2024.
//

#ifndef ARCADEGAMESGLFW_SNAKE_H
#define ARCADEGAMESGLFW_SNAKE_H


#include <deque>
#include "SnakeMap.h"
#include "TileEngine.h"
#include "Button.h"

template<typename T=unsigned short>
struct SnakeBody : public Coords<T>{
    SnakeBody(unsigned int xy, Directions direction) : Coords<T>(xy), direction(direction) {}

    SnakeBody(unsigned int x, unsigned int y, Directions direction) : Coords<T>(x, y), direction(direction) {}

    Directions direction {};
    std::shared_ptr<MergedRender> render {new MergedRender()};
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
    void useCoordsAndTextureAndLoad(SnakeBody<>&, std::shared_ptr<Texture>&);
    void loopPause();
    void Server();
    void loadResources();
    void MoveSnake(SnakeBody<> to);
    void Reset();
    void ResetApple();
    void SelectAndOpenMap();
    bool OpenMap(const std::string& path);
    std::deque<SnakeBody<>> snake {};
    Directions direction = Down;
    Coords<> AppleCoords;
    std::unique_ptr<SnakeMap> map;
    // TEMP
    bool Pause = false;
    float lastTickTime;
    int Score = 0;
    //AUDIO
    std::unique_ptr<Sound> Eat {new Sound()};
    // TEXTURES
    std::shared_ptr<Texture> AngleTexture {new Texture()};
    std::shared_ptr<Texture> TailTexture {new Texture()};
    std::shared_ptr<Texture> HeadTexture {new Texture()};
    std::shared_ptr<Texture> BodyTexture {new Texture()};
    std::shared_ptr<Texture> WallTexture {new Texture()};
    //TEXTURE COORDS
    const std::array<glm::vec2, 4> texturecordsUp = {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};
    const std::array<glm::vec2, 4> texturecordsLeft = {glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsDown = {glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0)};
    const std::array<glm::vec2, 4> texturecordsRight = {glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0),glm::vec2(1.0, 1.0)};
    // RENDER
    std::unique_ptr<MergedRender> Grid {new MergedRender()};
    std::unique_ptr<MergedRender> Apple {new MergedRender()};
    std::vector<std::unique_ptr<MergedRender>> WallsRender {};
    std::vector<std::unique_ptr<MergedRender>> ScoreBuffer = {};
    //UI
    std::vector<Button> pauseButtons {};
};


#endif //ARCADEGAMESGLFW_SNAKE_H
