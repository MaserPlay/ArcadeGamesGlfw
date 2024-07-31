//
// Created by super on 06.07.2024.
//

#include "IntroContext.h"
#include <ctime>
#include <glm/ext/matrix_clip_space.hpp>
#include "debug.h"
#include "System/SystemAdapter.h"
//STB
//DEFINE STBI
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//ARCHIVE
#include "System/ZipArchive.h"
//SOUND
#include "Audio/Sound.h"
#include "UI.h"

//DEFINES
#define IntroDuration 5 //s
#define BlinkDuration 1 //s
#define TickSpeed .3f //s


void IntroContext::init() {
    SetIcon("intro_icon.png");

    Grid->setSpeed(MergedRender::SpeedContent::STATIC);

    LogoRender->setFragmentShader(Shaders::TextureFragmentShader);
    LogoRender->setSpeed(MergedRender::SpeedContent::STATIC);
    LogoRender->quard.reset(new ExtendedQuard({-.7,-.5},1.4,1.4, std::make_shared<Texture>()));

    loadResources();
    loadSnakeResources();

    //LOAD ALL
    Grid->load();
    LogoRender->load();

    //INIT SNAKES
    for (int i = 0; i < 4; ++i) {
        Snakes.emplace_back();
    }
    for (short i = 0; i < 9; ++i) {
        Snakes[0].emplace_back(6, i, Directions::Down);
    }
    for (short i = -3; i < 3; ++i) {
        Snakes[1].emplace_back(i, -6, Directions::Left);
    }
    for (short i = 0; i > -8; --i) {
        Snakes[2].emplace_back(i, 4, Directions::Right);
    }
    for (short i = -2; i > -13; --i) {
        Snakes[3].emplace_back(i, 1, Directions::Right);
    }
    StartTime = ((float) clock() / CLOCKS_PER_SEC);
}

void IntroContext::loadResources() {
    INIT_ARCHIVE("Intro")

    LoadResources::loadImage(archive, "logo.png", LogoRender->quard->texture);

    CLOSE_ARCHIVE
}

void IntroContext::loadSnakeResources() {
    INIT_ARCHIVE("Snake")

    LoadResources::loadImage(archive, "body_angle.png", AngleTexture);
    LoadResources::loadImage(archive, "tail.png", TailTexture);
    LoadResources::loadImage(archive, "head.png", HeadTexture);
    LoadResources::loadImage(archive, "body.png", BodyTexture);
    LoadResources::loadFragmentShader(archive, "chess.frag", Grid);

    CLOSE_ARCHIVE
}

void IntroContext::MoveSnake(SnakeBodyUpdated to, std::deque<SnakeBodyUpdated> &snake) {
    auto tryTo = snake.front() + to;
//    if (tryTo.x <= 0)
//        tryTo.x = map->getField().x;
//    else if (tryTo.y <= 0)
//        tryTo.y = map->getField().y;
//    else if (tryTo.y >= map->getField().y + 1)
//        tryTo.y = 1;
//    else if (tryTo.x >= map->getField().x + 1)
//        tryTo.x = 1;
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

    snake.pop_back();
    snake.emplace_front(tryTo.x, tryTo.y, to.direction);
}

void IntroContext::loop() {
    START_LOOP

    //USE GRID
    Grid->useProgramm();
    Grid->SetUniform({getWidth(),getHeight()}, "u_resolution");
    Grid->SetUniform(projectionMatrix, "projection");
    Grid->SetUniform((screensize * 2), "field");
    Grid->useClear();

    for (auto& snake : Snakes) {
        for (auto& s :snake) {
            s.render->use(projectionMatrix);
        }
    }

    LogoRender->use(UIMatrix);

    if ((int) (((float) clock() / CLOCKS_PER_SEC) - StartTime)%2==1)
    {
        auto size = .009f;
        Font::RenderText("Present", getFont(), {(float) Font::TextWidth("Present", getFont()) * -size / 2,-.8}, UIMatrix, PresentsBuffer, size);
    }

    if (StartTime + IntroDuration <= ((float) clock() / CLOCKS_PER_SEC))
    {
        SPDLOG_DEBUG("Intro over");
        SkipIntro();
    }

    END_LOOP_G
    Server();
}

void IntroContext::Server() {
    //SERVER
    if (lastTickTime + TickSpeed <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        for (auto& s : Snakes) {
            switch(s.back().direction)
            {
                case Up:
                    MoveSnake(SnakeBodyUpdated(0,1, s.back().direction), s);
                    break;
                case Left:
                    MoveSnake(SnakeBodyUpdated(-1,0, s.back().direction), s);
                    break;
                case Down:
                    MoveSnake(SnakeBodyUpdated(0,-1, s.back().direction), s);
                    break;
                case Right:
                    MoveSnake(SnakeBodyUpdated(1,0, s.back().direction), s);
                    break;
                default:
                    SPDLOG_WARN("Unknown direction in SnakeTimer");
                    MoveSnake(SnakeBodyUpdated(0,1, s.back().direction), s);
                    break;
            }
        }
    }
}

void IntroContext::size_callback(int width, int height) {
    glViewport(0,0, width, height);
    float modifi = 8;
    screensize = glm::vec2{modifi};
    if ((float) width > (float) height) {
        screensize.x = modifi * ((float) width / (float) height) + 1;
        projectionMatrix = glm::ortho((double) -modifi * ((double) width/(double) height), (double) modifi * ((double) width/(double) height), (double) -modifi, (double) modifi);
    }
    else {
        screensize.y = modifi * ((float) height / (float) width) + 1;
        projectionMatrix = glm::ortho((double) -modifi, (double) modifi, (double) -modifi * ((double) height/(double) width), (double) modifi * ((double) height/(double) width));
    }
    Grid->quard.reset(new ExtendedQuard({-screensize},screensize.x * 2, screensize.y * 2));
    Grid->VerticesChanged();
    UIMatrix = UI::Matrix(width, height);
}

void IntroContext::SkipIntro() {
    loadMainMenu();
}

IntroContext::~IntroContext() {
}

void IntroContext::useCoordsAndTextureAndLoad(SnakeBodyUpdated & b, const std::shared_ptr<Texture>& t) {
    if (b.render->getShaderProgram() <= 0)
    {
        b.render->setFragmentShader(Shaders::TextureFragmentShader);
        b.render->quard = std::make_unique<ExtendedQuard>(b, 1, 1, t);
        b.render->setSpeed(MergedRender::SpeedContent::STREAM);
        b.render->load();
    } else {
        b.render->quard.reset(new ExtendedQuard(b,1,1,t));
        b.render->quard->texture = t;
        b.render->VerticesChanged();
    }
}

void IntroContext::key_callback(int key, int scancode, int action, int mods) {
    SkipIntro();
}


#undef IntroDuration