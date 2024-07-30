//
// Created by super on 17.06.2024.
//

#include "Snake.h"
#include <ctime>
#include <memory>
#include "debug.h"
#include "System/SystemAdapter.h"
//STB
//DEFINE STBI
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//ARCHIVE
#include "System/ZipArchive.h"
//SOUND
#include "System/SoundFile.hpp"
#include "System/Localization.h"
#include "UI.h"

using namespace TileEngineUtils;


void Snake::init() {
    glfwSetWindowTitle(getwindow(), "Snake");
    SetIcon("snake_icon.png");

    Grid->setSpeed(MergedRender::SpeedContent::STATIC);
    Grid->quard.reset(new MergedRender::Quard{{1,1}, 1, 1});

    Apple->setFragmentShader(MergedRender::TextureFragmentShader);
    Apple->setSpeed(MergedRender::SpeedContent::DYNAMIC);
    Apple->quard.reset(new MergedRender::Quard{{1,1}, 1, 1, std::make_shared<Texture>()});

    UI::DarkerBackground(DarkBack);

    loadResources();

    //INIT BUTTONS
    pauseButtons.emplace_back([](){loadMainMenu();}, ("Quit"));
    pauseButtons.emplace_back([this](){SelectAndOpenMap();}, ("OpenMap"));
    pauseButtons.emplace_back([this](){Reset();Pause = false;}, ("Restart"));
    pauseButtons.emplace_back([this](){Pause = false;}, ("Resume"));


    //LOAD ALL
    Grid->load();
    DarkBack->load();
    Apple->load();
    ButtonVBox::Init(pauseButtons);

    if (!OpenMap(SystemAdapter::GetGameFolderName("Snake") + "Default.smap"))
    {
        if (boxer::show(_c("Failed to open default map. Regenerate it?"), _c("Failed to open map"), boxer::Style::Error, boxer::Buttons::YesNo) == boxer::Selection::No)
        {
            loadMainMenu();
            return;
        }
        map = std::make_unique<SnakeMap>();
        SnakeMap::save(SystemAdapter::GetGameFolderName("Snake") + "Default.smap", map.get());
        OpenMap(SystemAdapter::GetGameFolderName("Snake") + "Default.smap");
    }
}

bool Snake::OpenMap(const std::string& path) {
    if (path.empty())
    {
        return false;
    }
    glfwSetWindowTitle(getwindow(), ( _("Snake. Opened map: ...") + path.substr(path.find_last_of('\\'),path.size() - path.find_last_of('\\'))).c_str());
    auto m = SnakeMap::load(path);
    if (m == nullptr)
    {
        return false;
    } else {
        map.reset(m);
    }
    Reset();
    size_callback(getWidth(), getHeight());
    Grid->quard.reset(new MergedRender::Quard{{1, 1},(float) m->getField().x, (float) m->getField().y});
    Grid->VerticesChanged();


    for (unsigned int i = 0; i < map->getMap().size(); ++i) {
        switch (map->getMap()[i]) {
            case SnakeMap::None:
                break;
            case SnakeMap::Wall: {
                WallsRender.emplace_back();
                std::unique_ptr<MergedRender>& r = WallsRender.back();
                r->setSpeed(MergedRender::SpeedContent::STATIC);
                r->quard.reset(new MergedRender::Quard({(i % map->getField().x) + 1,(i/map->getField().x) + 1}, 1, 1, WallTexture));
                r->load();
                break;
            }
            default:
                SPDLOG_WARN("Cannot find tile type in render");
                break;
        }
    }

    return true;
}

void Snake::loadResources() {
    INIT_ARCHIVE("Snake")

    TileEngineUtils::LoadResources::loadImage(archive, "apple.png", Apple->quard->texture);
    TileEngineUtils::LoadResources::loadImage(archive, "body_angle.png", AngleTexture);
    TileEngineUtils::LoadResources::loadImage(archive, "tail.png", TailTexture);
    TileEngineUtils::LoadResources::loadImage(archive, "head.png", HeadTexture);
    TileEngineUtils::LoadResources::loadImage(archive, "body.png", BodyTexture);
    TileEngineUtils::LoadResources::loadImage(archive, "wall.png", WallTexture);
    TileEngineUtils::LoadResources::loadAudio(archive, "eat.wav", Eat);
    TileEngineUtils::LoadResources::loadFragmentShader(archive, "chess.frag", Grid);

    CLOSE_ARCHIVE
}
void Snake::Reset() {
    direction = map->getDefaultSnakeDirection();
    Score = 0;
    snake.clear();
    switch (map->getDefaultSnakeDirection()) {
        case Up:
            for (unsigned short i = map->getDefaultSnakeSize() + map->getDefaultSnakePos().y; i > map->getDefaultSnakePos().y; --i) {
                snake.emplace_back(map->getDefaultSnakePos().x, i, map->getDefaultSnakeDirection());
            }
            break;
        case Left:
            for (unsigned short i = map->getDefaultSnakePos().y; i < map->getDefaultSnakeSize() + map->getDefaultSnakePos().y; ++i) {
                snake.emplace_back(i, map->getDefaultSnakePos().x, map->getDefaultSnakeDirection());
            }
            break;
        case Down:
            for (unsigned short i = map->getDefaultSnakePos().y; i < map->getDefaultSnakeSize() + map->getDefaultSnakePos().y; ++i) {
                snake.emplace_back(map->getDefaultSnakePos().x, i, map->getDefaultSnakeDirection());
            }
            break;
        case Right:
            for (unsigned short i = map->getDefaultSnakeSize() + map->getDefaultSnakePos().y; i > map->getDefaultSnakePos().y; --i) {
                snake.emplace_back(i, map->getDefaultSnakePos().x, map->getDefaultSnakeDirection());
            }
            break;
    }
    ResetApple();
    lastTickTime = 0;
}
void Snake::ResetApple() {
    AppleCoords = Coords<>(std::rand() % map->getField().x + 1, std::rand() % map->getField().y + 1);
    for (auto& s : snake) {
        if (AppleCoords == s)
        {
            ResetApple();
            return;
        }
    }
    for (int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == SnakeMap::Wall && AppleCoords == Coords<>((i % map->getField().x) + 1, (i / map->getField().x) + 1)) {
            ResetApple();
            return;
        }
    }
    Apple->quard->setVertices(AppleCoords, 1, 1);
    Apple->VerticesChanged();
}


void Snake::MoveSnake(SnakeBody<> to) {
    auto tryTo = snake.front() + to;
    if (tryTo.x <= 0)
        tryTo.x = map->getField().x;
    else if (tryTo.y <= 0)
        tryTo.y = map->getField().y;
    else if (tryTo.y >= map->getField().y + 1)
        tryTo.y = 1;
    else if (tryTo.x >= map->getField().x + 1)
        tryTo.x = 1;
    auto crash = [=](){
        switch (boxer::show(_c("The snake crashed into an obstacle. Do you want to reset field?"), _c("The snake crashed into an obstacle"), boxer::Buttons::YesNo)) {
            case boxer::Selection::Yes: {
                Reset();
                break;
            }
            case boxer::Selection::No: {
                loadMainMenu();
                break;
            }
            default: {
                SPDLOG_WARN("You forgot to add case, in crashed snake dialog");
                loadMainMenu();
                break;
            }
        }
        return;
    };
    for (int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == SnakeMap::Wall) {
            if (CheckCollision(tryTo, Coords<>((i % map->getField().x) + 1,(i/map->getField().x) + 1)))
            {
                crash();
                return;
            }
        }
    }
    for (auto& s : snake) {
        if (CheckCollision(tryTo, s))
        {
            crash();
            return;
        }
    }
    //RENDER
    switch (snake.front().direction)
    {
        case Up:
            HeadTexture->texturecords = texturecordsUp;
            break;
        case Left:
            HeadTexture->texturecords = texturecordsLeft;
            break;
        case Down:
            HeadTexture->texturecords = texturecordsDown;
            break;
        case Right:
            HeadTexture->texturecords = texturecordsRight;
            break;
    }
    useCoordsAndTextureAndLoad(snake.front(), HeadTexture);
    for (int i = 1; i < snake.size() - 1; ++i) {
        if (snake[i].direction != snake[i - 1].direction) {
            if ((snake[i].direction == Directions::Left && snake[i - 1].direction == Directions::Up) ||
                (snake[i - 1].direction == Directions::Right && snake[i].direction == Directions::Down)) {
                AngleTexture->texturecords = texturecordsUp;
            } else if ((snake[i].direction == Directions::Right && snake[i - 1].direction == Directions::Up) ||
                       (snake[i - 1].direction == Directions::Left && snake[i].direction == Directions::Down)) {
                AngleTexture->texturecords = texturecordsLeft;
            } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Left) ||
                       (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Right)) {
                AngleTexture->texturecords = texturecordsDown;
            } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Right) ||
                       (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Left)) {
                AngleTexture->texturecords = texturecordsRight;
            }
            useCoordsAndTextureAndLoad(snake[i], AngleTexture);
            continue;
        }
        if (snake[i].direction == Directions::Right || snake[i].direction == Directions::Left) {
            BodyTexture->texturecords = texturecordsUp;
        } else {
            BodyTexture->texturecords = texturecordsLeft;
        }
        useCoordsAndTextureAndLoad(snake[i], BodyTexture);
    }
    switch (snake[snake.size() - 3].direction)
    {
        case Up:
            TailTexture->texturecords = texturecordsUp;
            break;
        case Left:
            TailTexture->texturecords = texturecordsLeft;
            break;
        case Down:
            TailTexture->texturecords = texturecordsDown;
            break;
        case Right:
            TailTexture->texturecords = texturecordsRight;
            break;
    }
    useCoordsAndTextureAndLoad(snake[snake.size() - 2], TailTexture);

    if (tryTo == AppleCoords)
    {
        ResetApple();
        Eat->Play();
        Score++;
    } else {
        snake.pop_back();
    }
    snake.emplace_front(tryTo.x, tryTo.y, to.direction);
}

void Snake::loop() {
    Server();
    START_LOOP

    //USE GRID
    Grid->useProgramm();
    Grid->SetUniform({getWidth(),getHeight()}, "u_resolution");
    Grid->SetUniform(projectionMatrix, "projection");
    Grid->SetUniform((map->getField()), "field");
    Grid->useClear();

    for (auto& s :snake) {
        s.render->use(projectionMatrix);
    }

    for(auto& r : WallsRender){
        r->use(projectionMatrix);
    }

    Apple->use(projectionMatrix);
    Font::RenderText(std::to_string(Score), {-.9,-.9}, UIMatrix, ScoreBuffer, .005f);
    if (Pause)
    {
        loopPause();
    }

    END_LOOP
}

void Snake::Server() {
    //SERVER
    if (!Pause && lastTickTime + map->getTickSpeed() <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        switch(direction)
        {
            case Up:
                MoveSnake(SnakeBody(0,1, direction));
                break;
            case Left:
                MoveSnake(SnakeBody(-1,0, direction));
                break;
            case Down:
                MoveSnake(SnakeBody(0,-1, direction));
                break;
            case Right:
                MoveSnake(SnakeBody(1,0, direction));
                break;
            default:
                SPDLOG_WARN("Unknown direction in SnakeTimer");
                MoveSnake(SnakeBody(0,1, direction));
                break;
        }
    }
}

void Snake::loopPause() {
    DarkBack->use(UIMatrix);
    ButtonVBox::Show(pauseButtons, UIMatrix);
}

void Snake::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
    {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                Pause = !Pause;
                break;
            case GLFW_KEY_W:
                direction = Directions::Up;
                break;
            case GLFW_KEY_A:
                direction = Directions::Left;
                break;
            case GLFW_KEY_S:
                direction = Directions::Down;
                break;
            case GLFW_KEY_D:
                direction = Directions::Right;
                break;
            case GLFW_KEY_F11:
                SwitchFullscreen();
                break;
            case GLFW_KEY_I:
                Info(_c("This is my first game in c++; opengl. There are bugs in this game. If you find them, please write, admin@maserplay.ru ."))
                break;
        }
    }
}

void Snake::size_callback(int width, int height) {
    TileEngine::size_callback(width, height, map->getField());
    UIMatrix = UI::Matrix(width, height);
}

Snake::~Snake() = default;


void Snake::mouse_button_callback(int button, int action, int mods) {
    if (Pause && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        ButtonVBox::Click(pauseButtons);
    }
}

void Snake::SelectAndOpenMap() {
    if (!OpenMap(SystemAdapter::OpenFileDialog({{_c("Snake map"), "smap"}},
                                              SystemAdapter::GetGameFolderName("Snake"))))
    {
        boxer::show(_c("Failed to open map"), _c("Failed to open map"), boxer::Style::Error);
    }
}

void Snake::useCoordsAndTextureAndLoad(SnakeBody<> & b, std::shared_ptr<Texture>& t) {
    if (b.render->getShaderProgram() <= 0)
    {
        b.render->setFragmentShader(MergedRender::TextureFragmentShader);
        b.render->quard = std::make_unique<MergedRender::Quard>(b, 1, 1, t);
        b.render->setSpeed(MergedRender::SpeedContent::STREAM);
        b.render->load();
    } else {
        b.render->quard->setVertices(b, 1, 1);
        b.render->quard->texture = t;
        b.render->VerticesChanged();
    }
}