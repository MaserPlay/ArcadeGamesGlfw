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
    //setIcon();

    map = PacmanMap::load(SystemAdapter::GetGameFolderName("Pacman") + "Default.pmap");
    if (map == nullptr)
    {
        if (boxer::show("Failed to open default map. Regenerate it?", "Failed to open map", boxer::Style::Error, boxer::Buttons::YesNo) == boxer::Selection::No)
        {
            loadMainMenu();
            return;
        }
        map = new PacmanMap();
        PacmanMap::save(SystemAdapter::GetGameFolderName("Pacman") + "Default.pmap", map);
    }

    Reset();
    initEngine();
    loadResources();
}

void Pacman::loadResources() {
    SPDLOG_INFO("Start loading resources...");
    auto zippath = SystemAdapter::GetGameFolderName("Pacman") + "Pacman_resources.zip";
    if (!std::filesystem::is_regular_file(zippath)){
        SPDLOG_WARN("archive with resources {} not found", zippath);
        return;
    }
    auto archive = ZipArchive(zippath);
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
    lastTickTime = 0;
    pacmanPos = map->getStartPos();
    MoventGrid.clear();
    for (unsigned int i = 0; i < map->getMap().size(); ++i) {
        if (map->getMap()[i] == PacmanMap::None)
        {
            MoventGrid.emplace_back((i % map->getField().x) + 1,(i/map->getField().x) + 1);
        }
    }
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
                renderTile(Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1), new Texture(), {0,0,1.,1.});
                break;
            }
            default:
                SPDLOG_WARN("Cannot find tile type in render {}", (unsigned int) map->getMap()[i]);
                break;
        }
    }
    renderTile(pacmanPos, new Texture(), {1., 1., 0, 1.});

    glfwSwapBuffers(getwindow());
    if (lastTickTime + tickSpeed <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        switch (direction) {
            case Up:
                if (CheckCollision(Coords(pacmanPos.x + 0,pacmanPos.y + 1)))
                {
                    pacmanPos.y++;
                }
                break;
            case Left:
                if (CheckCollision(Coords(pacmanPos.x + -1,pacmanPos.y + 0)))
                {
                    pacmanPos.x--;
                }
                break;
            case Down:
                if (CheckCollision(Coords(pacmanPos.x + 0,pacmanPos.y + 1)))
                {
                    pacmanPos.y--;
                }
                break;
            case Right:
                if (CheckCollision(Coords(pacmanPos.x + 1,pacmanPos.y + 0)))
                {
                    pacmanPos.x++;
                }
                break;
            default:
                SPDLOG_WARN("Unknown direction");
                if (CheckCollision(Coords(pacmanPos.x + 0,pacmanPos.y + 1)))
                {
                    pacmanPos.y++;
                }
                break;
        }
        //SERVER LOGIC
    }
}

void Pacman::size_callback(int width, int height) {
    TileEngine::size_callback(width, height, map->getField());
}

void Pacman::key_callback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                loadMainMenu();
                break;
            case GLFW_KEY_F11:
                SwitchFullscreen();
                break;
            case GLFW_KEY_W:
                if (CheckCollision(Coords(pacmanPos.x + 0,pacmanPos.y + 1))) { direction = Directions::Up; }
                break;
            case GLFW_KEY_A:
                if (CheckCollision(Coords(pacmanPos.x + -1,pacmanPos.y + 0))) { direction = Directions::Left; }
                break;
            case GLFW_KEY_S:
                if (CheckCollision(Coords(pacmanPos.x + 0,pacmanPos.y + 1))) { direction = Directions::Down; }
                break;
            case GLFW_KEY_D:
                if (CheckCollision(Coords(pacmanPos.x + 1,pacmanPos.y + 0))) { direction = Directions::Right; }
                break;
            case GLFW_KEY_O: {
                auto path =SystemAdapter::OpenFileDialog({{"Pacman map", "pmap"}},
                                                         SystemAdapter::GetGameFolderName("Pacman"));
                if (path.empty())
                {
                    return;
                }
                glfwSetWindowTitle(getwindow(), ("Pacman. Opened map: ..." + path.substr(path.find_last_of('\\'),path.size() - path.find_last_of('\\'))).c_str());
                auto m = PacmanMap::load(path);
                if (m == nullptr)
                {
                    boxer::show("Failed to open map", "Failed to open map", boxer::Style::Error);
                } else {
                    map = m;
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

Pacman::~Pacman() {
    delete map;
}

bool Pacman::CheckCollision(Coords c) {
    for (auto& tile: MoventGrid) {
        if (tile == c)
        {
            return true;
        }
    }
    return false;
}

void Pacman::cursor_position_callback(double xpos, double ypos) {

}

void Pacman::mouse_button_callback(int button, int action, int mods) {

}
