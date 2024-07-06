//
// Created by super on 06.07.2024.
//

#include "IntroContext.h"
#include <ctime>
#include "debug.h"
#include "SystemAdapter.h"
//STB
//DEFINE STBI
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//ARCHIVE
#include "ZipArchive.h"
//SOUND
#include "Sound.h"

//DEFINES
#define IntroDuration 5 //s
#define BlinkDuration 1 //s


void IntroContext::init() {
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_ALPHA_TEST);
//    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    SetIcon("intro_icon.png");
    loadResources();
    loadSnakeResources();
    t = std::thread(InitAsync);
    //INIT SNAKES
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
    SPDLOG_INFO("Start loading resources...");
    auto zippath = SystemAdapter::GetGameFolderName("Intro") + "intro_resources.zip";
    if (!std::filesystem::is_regular_file(zippath)){
        SPDLOG_WARN("archive with resources {} not found", zippath);
        SkipIntro();
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
                texture = new Texture(image, width, height, Texture::Modes::RGB);
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

    loadImage("logo.png", LogoTexture);
    loadImage("presents.png", PresentsTexture);
}

void IntroContext::loadSnakeResources() {
    SPDLOG_INFO("Start loading snake resources...");
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
                texture = new Texture(image, width, height, Texture::Modes::RGB);
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

    loadImage("body_angle.png", AngleTexture);
    loadImage("tail.png", TailTexture);
    loadImage("head.png", HeadTexture);
    loadImage("body.png", BodyTexture);
}

void IntroContext::loop() {
    glfwPollEvents();
    glBindTexture(GL_TEXTURE_2D, 0);
    for (int x = -screensize.x; x < screensize.x; ++x) {
        for (int y = -screensize.y; y < screensize.y; ++y) {
            if (x%2==0)
            {
                if (y%2==0)
                {
                    glColor4d(0., .5, 0., 1.);
                    glBegin(GL_QUADS);
                    glVertex2d(x + 1,y);
                    glVertex2d(x + 1,y + 1);
                    glVertex2d(x,y + 1);
                    glVertex2d(x,y);
                    glEnd();
                } else {
                    glColor4d(0., 1., 0., 1.);
                    glBegin(GL_QUADS);
                    glVertex2d(x + 1,y);
                    glVertex2d(x + 1,y + 1);
                    glVertex2d(x,y + 1);
                    glVertex2d(x,y);
                    glEnd();
                }
            } else {
                if (y%2==0)
                {
                    glColor4d(0., 1., 0., 1.);
                    glBegin(GL_QUADS);
                    glVertex2d(x + 1,y);
                    glVertex2d(x + 1,y + 1);
                    glVertex2d(x,y + 1);
                    glVertex2d(x,y);
                    glEnd();
                } else {
                    glColor4d(0., .5, 0., 1.);
                    glBegin(GL_QUADS);
                    glVertex2d(x + 1,y);
                    glVertex2d(x + 1,y + 1);
                    glVertex2d(x,y + 1);
                    glVertex2d(x,y);
                    glEnd();
                } //TODO: БРЕД КАКОЙ ТО
            }
        }
    }

    for (auto& snake :Snakes) {
        switch (snake.front().direction)
        {
            case Up:
                HeadTexture->texturecords = texturecordsUp;
                renderTile(snake.front(), HeadTexture, {1.,0,0, 1.});
                break;
            case Left:
                HeadTexture->texturecords = texturecordsLeft;
                renderTile(snake.front(), HeadTexture, {1.,0,0, 1.});
                break;
            case Down:
                HeadTexture->texturecords = texturecordsDown;
                renderTile(snake.front(), HeadTexture, {1.,0,0, 1.});
                break;
            case Right:
                HeadTexture->texturecords = texturecordsRight;
                renderTile(snake.front(), HeadTexture, {1.,0,0, 1.});
                break;
        }
        for (int i = 1; i < snake.size() - 1; ++i) {
            if (snake[i].direction != snake[i - 1].direction) {
                if ((snake[i].direction == Directions::Left && snake[i - 1].direction == Directions::Up) ||
                    (snake[i - 1].direction == Directions::Right && snake[i].direction == Directions::Down)) {
                    AngleTexture->texturecords = texturecordsUp;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.});
                } else if ((snake[i].direction == Directions::Right && snake[i - 1].direction == Directions::Up) ||
                           (snake[i - 1].direction == Directions::Left && snake[i].direction == Directions::Down)) {
                    AngleTexture->texturecords = texturecordsLeft;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.});
                } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Left) ||
                           (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Right)) {
                    AngleTexture->texturecords = texturecordsDown;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.});
                } else if ((snake[i].direction == Directions::Up && snake[i - 1].direction == Directions::Right) ||
                           (snake[i - 1].direction == Directions::Down && snake[i].direction == Directions::Left)) {
                    AngleTexture->texturecords = texturecordsRight;
                    renderTile(snake[i], AngleTexture, {1., 0, 0, 1.});
                }
                continue;
            }
            if (snake[i].direction == Directions::Right || snake[i].direction == Directions::Left) {
                BodyTexture->texturecords = texturecordsUp;
                renderTile(snake[i], BodyTexture, {1.,0,0, 1.});
            } else {
                BodyTexture->texturecords = texturecordsLeft;
                renderTile(snake[i], BodyTexture, {1.,0,0, 1.});
            }
        }
        switch (snake[snake.size() - 2].direction)
        {
            case Up:
                TailTexture->texturecords = texturecordsUp;
                renderTile(snake.back(), TailTexture, {1.,0,0, 1.});
                break;
            case Left:
                TailTexture->texturecords = texturecordsLeft;
                renderTile(snake.back(), TailTexture, {1.,0,0, 1.});
                break;
            case Down:
                TailTexture->texturecords = texturecordsDown;
                renderTile(snake.back(), TailTexture, {1.,0,0, 1.});
                break;
            case Right:
                TailTexture->texturecords = texturecordsRight;
                renderTile(snake.back(), TailTexture, {1.,0,0, 1.});
                break;
        }
    }

    glColor4d(1.,1.,1., 1.);
    glBindTexture(GL_TEXTURE_2D, LogoTexture->getInitImage());
    glBegin(GL_QUADS);
    glTexCoord2d(LogoTexture->texturecords[0].x, LogoTexture->texturecords[0].y);
    glVertex2d(5,-5);
    glTexCoord2d(LogoTexture->texturecords[1].x, LogoTexture->texturecords[1].y);
    glVertex2d(5,5);
    glTexCoord2d(LogoTexture->texturecords[2].x, LogoTexture->texturecords[2].y);
    glVertex2d(-5,5);
    glTexCoord2d(LogoTexture->texturecords[3].x, LogoTexture->texturecords[3].y);
    glVertex2d(-5,-5);
    glEnd();

    if ((int) (((float) clock() / CLOCKS_PER_SEC) - StartTime)%2==1)
    {
//        SPDLOG_TRACE(((float) clock() / CLOCKS_PER_SEC) - StartTime);
        glColor4d(1.,1.,1., 1.);
        glBindTexture(GL_TEXTURE_2D, PresentsTexture->getInitImage());
        glBegin(GL_QUADS);
        glTexCoord2d(LogoTexture->texturecords[0].x, LogoTexture->texturecords[0].y);
        glVertex2d(5,(((float) PresentsTexture->getHeight() / (float) PresentsTexture->getWidth()) * -5) - 6);
        glTexCoord2d(LogoTexture->texturecords[1].x, LogoTexture->texturecords[1].y);
        glVertex2d(5,(((float) PresentsTexture->getHeight() / (float) PresentsTexture->getWidth()) * 5) - 6);
        glTexCoord2d(LogoTexture->texturecords[2].x, LogoTexture->texturecords[2].y);
        glVertex2d(-5,(((float) PresentsTexture->getHeight() / (float) PresentsTexture->getWidth()) * 5) - 6);
        glTexCoord2d(LogoTexture->texturecords[3].x, LogoTexture->texturecords[3].y);
        glVertex2d(-5,(((float) PresentsTexture->getHeight() / (float) PresentsTexture->getWidth()) * -5) - 6);
        glEnd();
    }

    if (StartTime + IntroDuration <= ((float) clock() / CLOCKS_PER_SEC))
    {
        SPDLOG_DEBUG("Intro over");
        SkipIntro();
    }

    glfwSwapBuffers(getwindow());
    //SERVER
    if (lastTickTime + .4 <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        for (auto& snake : Snakes) {
            snake.pop_back();
            switch(snake[0].direction)
            {
                case Up:
                    snake.emplace_front(snake.front().x, snake.front().y + 1, snake[0].direction);
                    break;
                case Left:
                    snake.emplace_front(snake.front().x + -1, snake.front().y, snake[0].direction);
                    break;
                case Down:
                    snake.emplace_front(snake.front().x, snake.front().y + -1, snake[0].direction);
                    break;
                case Right:
                    snake.emplace_front(snake.front().x + 1, snake.front().y, snake[0].direction);
                    break;
                default:
                    SPDLOG_WARN("Unknown direction in SnakeTimer");
                    snake.emplace_front(snake.front().x, snake.front().y + 1, snake[0].direction);
                    break;
            }
        }
    }
}

void IntroContext::size_callback(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double modifi = 8;
    screensize.x = 8; screensize.y = 8;
    if ((float) width > (float) height) {
        screensize.x = modifi * ((float) width / (float) height) + 1;
        glOrtho(-modifi * ((float) width / (float) height), modifi * ((float) width / (float) height), -modifi, modifi,
                -1.0, 1.0);
    }
    else {
        screensize.y = modifi * ((float) height / (float) width) + 1;
        glOrtho(-modifi, modifi, -modifi * ((float) height / (float) width), modifi * ((float) height / (float) width),
                -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void IntroContext::key_callback(int key, int scancode, int action, int mods) {

}

void IntroContext::SkipIntro() {
    if (t.joinable()) {
        t.join();
    }
    loadMainMenu();
}

void IntroContext::renderTile(glm::vec<2, short> coords, Texture *t, glm::vec4 c) {
    if (t->getInitImage() == 0 ) {glColor4d(c.r,c.g,c.b, c.a);} else {glColor4d(1.,1.,1., 1.);}
    glBindTexture(GL_TEXTURE_2D, t->getInitImage());
    glBegin(GL_QUADS);
    glTexCoord2d(t->texturecords[0].x, t->texturecords[0].y);
    glVertex2d(coords.x + 1,coords.y);
    glTexCoord2d(t->texturecords[1].x, t->texturecords[1].y);
    glVertex2d(coords.x + 1,coords.y + 1);
    glTexCoord2d(t->texturecords[2].x, t->texturecords[2].y);
    glVertex2d(coords.x,coords.y + 1);
    glTexCoord2d(t->texturecords[3].x, t->texturecords[3].y);
    glVertex2d(coords.x,coords.y);
    glEnd();
}

#undef IntroDuration