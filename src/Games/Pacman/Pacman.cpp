//
// Created by super on 22.06.2024.
//

#include "Pacman.h"
#include <ctime>
#include "debug.h"
#include "System/SystemAdapter.h"
//STB
#include "stb_image.h"
//ARCHIVE
#include "System/ZipArchive.h"
#include "System/SoundFile.hpp"
//MAP
#include "PacmanMap.h"
#include "System.h"

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
    loadResources();
}

void Pacman::loadResources() {
    INIT_ARCHIVE("Pacman")

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
//                TileEngine::renderTile(Coords((i % map->getField().x) + 1,(i/map->getField().x) + 1), new Texture(), {0,0,1.,1.});
                break;
            }
            default:
                SPDLOG_WARN("Cannot find tile type in render {}", (unsigned int) map->getMap()[i]);
                break;
        }
    }
//    TileEngine::renderTile(pacmanPos, new Texture(), {1., 1., 0, 1.});

    glfwSwapBuffers(getwindow());
    if (lastTickTime + tickSpeed <= ((float) clock() / CLOCKS_PER_SEC)) {
        lastTickTime = ((float) clock() / CLOCKS_PER_SEC);
        switch (direction) {
            case Up:
                if (CheckCollision(Coords<>(pacmanPos.x + 0,pacmanPos.y + 1)))
                {
                    pacmanPos.y++;
                }
                break;
            case Left:
                if (CheckCollision(Coords<>(pacmanPos.x + -1,pacmanPos.y + 0)))
                {
                    pacmanPos.x--;
                }
                break;
            case Down:
                if (CheckCollision(Coords<>(pacmanPos.x + 0,pacmanPos.y + 1)))
                {
                    pacmanPos.y--;
                }
                break;
            case Right:
                if (CheckCollision(Coords<>(pacmanPos.x + 1,pacmanPos.y + 0)))
                {
                    pacmanPos.x++;
                }
                break;
            default:
                SPDLOG_WARN("Unknown direction");
                if (CheckCollision(Coords<>(pacmanPos.x + 0,pacmanPos.y + 1)))
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
                if (CheckCollision(Coords<>(pacmanPos.x + 0,pacmanPos.y + 1))) { direction = Directions::Up; }
                break;
            case GLFW_KEY_A:
                if (CheckCollision(Coords<>(pacmanPos.x + -1,pacmanPos.y + 0))) { direction = Directions::Left; }
                break;
            case GLFW_KEY_S:
                if (CheckCollision(Coords<>(pacmanPos.x + 0,pacmanPos.y + 1))) { direction = Directions::Down; }
                break;
            case GLFW_KEY_D:
                if (CheckCollision(Coords<>(pacmanPos.x + 1,pacmanPos.y + 0))) { direction = Directions::Right; }
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

bool Pacman::CheckCollision(Coords<> c) {
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
