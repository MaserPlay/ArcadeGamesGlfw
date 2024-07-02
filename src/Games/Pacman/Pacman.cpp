//
// Created by super on 22.06.2024.
//

#include "Pacman.h"
#include <ctime>
#include "debug.h"
#include "SystemAdapter.h"
//STB
#include "stb_image.h"
//ARCHIVE
#include "ZipArchive.h"
#include "SoundFile.hpp"
//MAP
#include "PacmanMap.h"

void Pacman::init() {
    glfwSetWindowTitle(getwindow(), "Pacman");

    map = PacmanMap::load(SystemAdapter::GetGameFolderName("Pacman") + "\\Default.pmap");
    if (map == nullptr)
    {
        map = new PacmanMap();
        PacmanMap::save(SystemAdapter::GetGameFolderName("Pacman") + "\\Default.pmap", map);
    }

    Reset();
    if (map->getField().x <= 0 || map->getField().y <= 0)
    {
        ErrorAbort("[Pacman] field.x <= 0 || field.y <= 0")
    }

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glLoadIdentity();
    loadResources();
}

void Pacman::loadResources() {
    spdlog::info("[PACMAN] Start loading resources...");
    if (!std::filesystem::is_regular_file(SystemAdapter::GetGameFolderName("Pacman") + "Pacman_resources.zip")){
        spdlog::warn("[SNAKE] archive with resources {} not found", SystemAdapter::GetGameFolderName("Pacman") + "Pacman_resources.zip");
        return;
    }
    auto archive = ZipArchive(SystemAdapter::GetGameFolderName("Pacman") + "Pacman_resources.zip");
    char *content = NULL; zip_uint64_t size; unsigned char *image = NULL;
    int width, height, nrChannels;

    auto loadImage = [&](const std::string& name, Texture*& texture){
        archive.get(name, content, size);
        if (content == NULL)
        {
            spdlog::warn("[PACMAN] " + name + " not found");
        } else {
            image = stbi_load_from_memory(reinterpret_cast<const unsigned char *const>(content), size, &width, &height,
                                          &nrChannels, 0);
            texture = new Texture(image, width, height);
            texture->Load();
            //SOIL_free_image_data(image);
            spdlog::info("[PACMAN] " + name + " loaded");
        }
    };
    auto loadAudio = [&](const std::string& name, ALuint& buffer){
        archive.get(name, content, size);
        if (content == NULL) {
            spdlog::warn("[PACMAN] " + name + " not found");
        } else {
            auto sf = new SoundFile();
            sf->openRead(content, size);
            ALenum format;
            if (sf->getChannelCount() == 1)
                format = AL_FORMAT_MONO16;
            else if (sf->getChannelCount() == 2)
                format = AL_FORMAT_STEREO16;
            else {
                spdlog::error("ERROR: unrecognised wave format: {} channels", sf->getChannelCount());
                return;
            }
            alGenBuffers(1, &buffer);
            alBufferData(buffer, format, sf->getOpenAlData(), sf->getOpenAlDataSize(), sf->getSampleRate());
            delete sf;
            spdlog::info("[PACMAN] " + name + " loaded");
        }
    };

}

void Pacman::Reset() {

}

void Pacman::loop() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (unsigned int i = 0; i < map->getMap().size(); ++i) {
        switch (map->getMap()[i]) {
            case PacmanMap::None:
                break;
            case PacmanMap::Wall: {
                glColor4d(0,0,1., 1.);
                glBindTexture(GL_TEXTURE_2D, 0);
                renderTile(Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1));
                break;
            }
            default:
                spdlog::warn("[SNAKE] Cannot find tile type in render {}", (unsigned int) map->getMap()[i]);
                break;
        }
    }

    glfwSwapBuffers(getwindow());
    if (lastTickTime + tickSpeed <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        //SERVER LOGIC
    }
}

void Pacman::renderTile(Coords coords, std::array<glm::vec2, 4> v) {
    glBegin(GL_QUADS);
    glTexCoord2d(v[0].x, v[0].y);
    glVertex2d(coords.x + 1,coords.y);
    glTexCoord2d(v[1].x, v[1].y);
    glVertex2d(coords.x + 1,coords.y + 1);
    glTexCoord2d(v[2].x, v[2].y);
    glVertex2d(coords.x,coords.y + 1);
    glTexCoord2d(v[3].x, v[3].y);
    glVertex2d(coords.x,coords.y);
    glEnd();
}

void Pacman::size_callback(int width, int height) {
    glViewport(0,0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    auto screensize = glm::vec<2, unsigned int>(5,5);
    double Shift = 0;
    if ((float) width >= (float) height) {
        Shift = (((double) (width) / (double) (height)) - (double) screensize.x / (double) screensize.y) - 1;
        glOrtho( -Shift, (double) screensize.x * ((double) width / (double) height) - Shift, 0. + 1, (double) screensize.y + 1, 0., 1.);
    }
    else {
        Shift = (((double) (height) / (double) (width)) - (double) screensize.y / (double) screensize.x) - 1;
        glOrtho(0. + 1, (double) screensize.x + 1, -Shift, (double) screensize.y * ((double) height / (double) width) - Shift, 0., 1.);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Pacman::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                loadMainMenu();
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
            case GLFW_KEY_O: {
                auto path =SystemAdapter::OpenFileDialog({{"Pacman map", "pmap"}},
                                                         SystemAdapter::GetGameFolderName("Pacman"));
                if (path.empty())
                {
                    return;
                }
                glfwSetWindowTitle(getwindow(), ("Pacman. Opened map: ..." + path.substr(path.find_last_of('\\'),path.size() - path.find_last_of('\\'))).c_str());
                map = PacmanMap::load(path);
                if (map->getField().x <= 0 || map->getField().y <= 0)
                {
                    ErrorAbort("[PACMAN] field.x <= 0 || field.y <= 0")
                }
                if (map->getField().y != map->getField().x)
                {
                    ErrorAbort("[PACMAN] field.y != field.x, which is unsupported")
                }
                Reset();
                size_callback(getWidth(), getHeight());
                break;
            }
            case GLFW_KEY_I:
                Info("Pacman")
                break;
        }
    }
}

bool Pacman::CheckCollision(Coords c, Coords s) {
    return c == s;
}

Pacman::~Pacman() {
    delete map;
}
