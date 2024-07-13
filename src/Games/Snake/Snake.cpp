//
// Created by super on 17.06.2024.
//

#include "Snake.h"
#include <ctime>
#include "debug.h"
#include "SystemAdapter.h"
//STB
//DEFINE STBI
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//ARCHIVE
#include "ZipArchive.h"
//SOUND
#include "SoundFile.hpp"
#include "Localization.h"

void Snake::init() {
    glfwSetWindowTitle(getwindow(), "Snake");
    SetIcon("snake_icon.png");
//    Info("I - info about game\nO - open map\nEsc- Exit game\nWASD - movement")

    map = SnakeMap::load(SystemAdapter::GetGameFolderName("Snake") + "Default.smap");
    if (map == nullptr)
    {
        if (boxer::show(_c("Failed to open default map. Regenerate it?"), _c("Failed to open map"), boxer::Style::Error, boxer::Buttons::YesNo) == boxer::Selection::No)
        {
            loadMainMenu();
            return;
        }
        map = new SnakeMap();
        SnakeMap::save(SystemAdapter::GetGameFolderName("Snake") + "Default.smap", map);
    }
    Reset();
    initEngine();
    loadResources();
}

void Snake::loadResources() {
    SPDLOG_INFO("Start loading resources...");
    auto zippath = SystemAdapter::GetGameFolderName("Snake") + "Snake_resources.zip";
    if (!std::filesystem::is_regular_file(zippath)){
        SPDLOG_WARN("archive with resources {} not found", SystemAdapter::GetGameFolderName("Snake") + "Snake_resources.zip");
        return;
    }
    auto archive = ZipArchive(zippath);
    char *content = NULL; zip_uint64_t size;
    unsigned char *image = NULL; int width, height, nrChannels;

    auto loadImage = [&](const std::string& name, Texture*& texture){
        archive.get(name, content, size);
        if (content == NULL)
        {
            SPDLOG_WARN(name + " not found");
        } else {
            image = stbi_load_from_memory(reinterpret_cast<const unsigned char *const>(content), size, &width, &height,
                                          &nrChannels, 0);
            if (nrChannels == 4)
            {
                texture = new Texture(image, width, height);
            } else if (nrChannels == 3)
            {
                texture = new Texture(image, width, height, Texture::Colors::RGB);
            } else {
                SPDLOG_WARN("{} have {} channels. What is undefined", name, nrChannels);
            }
            texture->Load();
            stbi_image_free(content);
            SPDLOG_INFO(name + " loaded");
        }
    };
    auto loadAudio = [&](const std::string& name, Sound*& buffer){
        archive.get(name, content, size);
        if (content == NULL) {
            SPDLOG_WARN(name + " not found");
        } else {
            buffer = new Sound(content, size);
            SPDLOG_INFO(name + " loaded");
        }
    };

    loadImage("apple.png", AppleTexture);
    loadImage("body_angle.png", AngleTexture);
    loadImage("tail.png", TailTexture);
    loadImage("head.png", HeadTexture);
    loadImage("body.png", BodyTexture);
    loadImage("wall.png", WallTexture);
    loadAudio("eat.wav", Eat);
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
    Apple = Coords(std::rand() % map->getField().x + 1, std::rand() % map->getField().y + 1);
    for (auto& s : snake) {
        if (Apple == s)
        {
            ResetApple();
            return;
        }
    }
    for (int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == SnakeMap::Wall && Apple == Coords((i % map->getField().x) + 1, (i / map->getField().x) + 1)) {
            ResetApple();
            return;
        }
    }
}


void Snake::MoveSnake(SnakeBody to) {
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
            if (CheckCollision(tryTo, Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1)))
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
    if (tryTo == Apple)
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
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glBindTexture(GL_TEXTURE_2D, 0);
    //GRID
    for (int x = 1; x < map->getField().x + 1; ++x) {
        for (int y = 1; y < map->getField().y + 1; ++y) {
            if (x%2==0)
            {
                if (y%2==0)
                {
                    renderTile(Coords(x,y), new Texture(), {0., .5, 0., 1.}, -1);
                } else {
                    renderTile(Coords(x,y), new Texture(), {0., 1., 0., 1.}, -1);
                }
            } else {
                if (y%2==0)
                {
                    renderTile(Coords(x,y), new Texture(), {0., 1., 0., 1.}, -1);
                } else {
                    renderTile(Coords(x,y), new Texture(), {0., .5, 0., 1.}, -1);
                } //TODO: БРЕД КАКОЙ ТО
            }
        }
    }

    for (unsigned int i = 0; i < map->getMap().size(); ++i) {
        switch (map->getMap()[i]) {
            case SnakeMap::None:
                break;
            case SnakeMap::Wall: {
                renderTile(Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1), WallTexture, {0,0,1., 1.}, 0);
                break;
            }
            default:
                SPDLOG_WARN("Cannot find tile type in render");
                break;
        }
    }
#ifndef SNAKE_ANIMATION
    switch (snake.front().direction)
    {
        case Up:
            HeadTexture->texturecords = texturecordsUp;
            renderTile(snake.front(), HeadTexture, {1.,0,0, 1.}, 0);
            break;
        case Left:
            HeadTexture->texturecords = texturecordsLeft;
            renderTile(snake.front(), HeadTexture, {1.,0,0, 1.}, 0);
            break;
        case Down:
            HeadTexture->texturecords = texturecordsDown;
            renderTile(snake.front(), HeadTexture, {1.,0,0, 1.}, 0);
            break;
        case Right:
            HeadTexture->texturecords = texturecordsRight;
            renderTile(snake.front(), HeadTexture, {1.,0,0, 1.}, 0);
            break;
    }
    for (int i = 1; i < snake.size() - 1; ++i) {
#else
    for (int i = 0; i < snake.size() - 1; ++i) {
        if (i != 0) {
#endif
            if (snake[i].direction != snake[i - 1].direction) {
                if ((snake[i].direction == Directions::Left && snake[i - 1].direction == Directions::Up) ||
                    (snake[i - 1].direction == Directions::Right && snake[i].direction == Directions::Down)) {
                    AngleTexture->texturecords = texturecordsUp;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.}, 0);
                } else if ((snake[i].direction == Directions::Right && snake[i - 1].direction == Directions::Up) ||
                           (snake[i - 1].direction == Directions::Left && snake[i].direction == Directions::Down)) {
                    AngleTexture->texturecords = texturecordsLeft;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.}, 0);
                } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Left) ||
                           (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Right)) {
                    AngleTexture->texturecords = texturecordsDown;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.}, 0);
                } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Right) ||
                           (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Left)) {
                    AngleTexture->texturecords = texturecordsRight;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.}, 0);
                }
                continue;
            }
#ifdef SNAKE_ANIMATION
        }
#endif
        if (snake[i].direction == Directions::Right || snake[i].direction == Directions::Left) {
            BodyTexture->texturecords = texturecordsUp;
            renderTile(snake[i], BodyTexture, {1.,0,0, 1.}, 0);
        } else {
            BodyTexture->texturecords = texturecordsLeft;
            renderTile(snake[i], BodyTexture, {1.,0,0, 1.}, 0);
        }
    }
    switch (snake[snake.size() - 2].direction)
    {
        case Up:
            TailTexture->texturecords = texturecordsUp;
            renderTile(snake.back(), TailTexture, {1.,0,0, 1.}, 0);
            break;
        case Left:
            TailTexture->texturecords = texturecordsLeft;
            renderTile(snake.back(), TailTexture, {1.,0,0, 1.}, 0);
            break;
        case Down:
            TailTexture->texturecords = texturecordsDown;
            renderTile(snake.back(), TailTexture, {1.,0,0, 1.}, 0);
            break;
        case Right:
            TailTexture->texturecords = texturecordsRight;
            renderTile(snake.back(), TailTexture, {1.,0,0, 1.}, 0);
            break;
    }
    renderTile(Apple, AppleTexture, {.3,0,0,1}, 0);
#ifdef SNAKE_ANIMATION
    auto move = ((((float) clock() / (float) CLOCKS_PER_SEC) - lastTickTime) / map->getTickSpeed()); //NORMALIZED
    glColor4d(1., 0, 0, 1.);
    glBindTexture(GL_TEXTURE_2D, 0);
    switch (direction) {
        case Up:
            glBegin(GL_QUADS);
            glVertex2f((float) snake.front().x + 1.f, (float) snake.front().y + move);
            glVertex2f((float) snake.front().x + 1.f, (float) snake.front().y + 1.f  + move);
            glVertex2f((float) snake.front().x, (float) snake.front().y + 1.f + move);
            glVertex2f((float) snake.front().x , (float) snake.front().y  + move);
            glEnd();
            break;
        case Left:
            glBegin(GL_QUADS);
            glVertex2f((float) snake.front().x + 1.f + -move, (float) snake.front().y);
            glVertex2f((float) snake.front().x + 1.f + -move, (float) snake.front().y + 1.f );
            glVertex2f((float) snake.front().x + -move, (float) snake.front().y + 1.f);
            glVertex2f((float) snake.front().x + -move, (float) snake.front().y );
            glEnd();
            break;
        case Down:
            glBegin(GL_QUADS);
            glVertex2f((float) snake.front().x + 1.f, (float) snake.front().y + -move);
            glVertex2f((float) snake.front().x + 1.f, (float) snake.front().y + 1.f  + -move);
            glVertex2f((float) snake.front().x, (float) snake.front().y + 1.f + -move);
            glVertex2f((float) snake.front().x , (float) snake.front().y  + -move);
            glEnd();
            break;
        case Right:
            glBegin(GL_QUADS);
            glVertex2f((float) snake.front().x + 1.f + move, (float) snake.front().y);
            glVertex2f((float) snake.front().x + 1.f + move, (float) snake.front().y + 1.f );
            glVertex2f((float) snake.front().x + move, (float) snake.front().y + 1.f);
            glVertex2f((float) snake.front().x + move, (float) snake.front().y );
            glEnd();
            break;
    }
#endif
    Font::RenderText(std::to_string(Score), {2.,2.}, .1, .005f * (float) map->getField().y);
    if (Pause)
    {
        loopPause();
    }
    glfwSwapBuffers(getwindow());
    Server();
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
    glColor4d(0,0,0, .3);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBegin(GL_QUADS);
    glVertex3d(map->getField().x + 1,1, 1);
    glVertex3d(map->getField().x + 1,map->getField().y + 1, 1);
    glVertex3d(1,map->getField().y + 1, 1);
    glVertex3d(1,1, 1);
    glEnd();
    std::string list[] {"Quit", "OpenMap", "Restart", "Resume"};
    int max = std::size(list);
    for (int i = 0; i < max; ++i) {
        glColor4d(1.,1.,0, 1.);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBegin(GL_QUADS);
        glVertex3d(((double) map->getField().x / 4 * 3) + 1,((double) map->getField().y / (max * 2) + 1) * (i + 1.5), 1.1);
        glVertex3d(((double) map->getField().x / 4 * 3) + 1,((double) map->getField().y / (max * 2) + 1) * (i + 1), 1.1);
        glVertex3d(((double) map->getField().x / 4) + 1,((double) map->getField().y / (max * 2) + 1) * (i + 1), 1.1);
        glVertex3d(((double) map->getField().x / 4) + 1,((double) map->getField().y / (max * 2) + 1) * (i + 1.5), 1.1);
        glEnd();
        auto size = .001f * (float) map->getField().y;
        Font::RenderText(list[i], {(((float) Font::TextWidth(list[i]) * -size / 2) + ((float) map->getField().x / 2)) + 1,((double) map->getField().y / (max * 2) + 1) * (i + 1.25)}, 1.2 , size, {0,0,0,1});
    }
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
}

Snake::~Snake() {
    delete AppleTexture;
    delete HeadTexture;
    delete BodyTexture;
    delete AngleTexture;
    delete TailTexture;
    delete map;
    delete Eat;
}

void Snake::mouse_button_callback(int button, int action, int mods) {
    if (Pause && button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(getwindow(), &xpos, &ypos);
        auto mouse_pos = TileEngine::vector_to_screencoords(xpos, ypos, map->getField());
        std::function<void()> list[] = {[=](){loadMainMenu();}, [=](){
            auto path  =SystemAdapter::OpenFileDialog({{_c("Snake map"), "smap"}},
                                                     SystemAdapter::GetGameFolderName("Snake"));
            if (path.empty())
            {
                return;
            }
            glfwSetWindowTitle(getwindow(), ( _("Snake. Opened map: ...") + path.substr(path.find_last_of('\\'),path.size() - path.find_last_of('\\'))).c_str());
            auto m = SnakeMap::load(path);
            if (m == nullptr)
            {
                boxer::show(_c("Failed to open map"), _c("Failed to open map"), boxer::Style::Error);
            } else {
                map = m;
            }
            Reset();
            size_callback(getWidth(), getHeight());
            }, [=](){Reset();Pause = false;}, [=](){Pause = false;}};
        int max = std::size(list);
        for (int i = 0; i < max; ++i) {
            if (mouse_pos.x < ((double) map->getField().x / 4 * 3) + 1 && mouse_pos.x > ((double) map->getField().x / 4) + 1 && mouse_pos.y < ((double) map->getField().y / (max * 2) + 1) * (i + 1.5) && mouse_pos.y > ((double) map->getField().y / (max * 2) + 1) * (i + 1)) //X && Y
            {
                list[i]();
                break;
            }
        }
    }
}
