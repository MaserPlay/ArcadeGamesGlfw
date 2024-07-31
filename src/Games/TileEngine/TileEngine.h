//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_TILEENGINE_H
#define ARCADEGAMES_TILEENGINE_H


#include "Utils/BaseGame.h"
#include "Engine.h"
#include "Render/Texture.h"
#include "Audio/Sound.h"
#include "glm/vec4.hpp"
#include "System/ZipArchive.h"
#include "Render/MergedRender.h"
#include "System.h"

class TileEngine : public BaseGame {
protected:
    void size_callback(int width, int height, const Coords<> screensize);
    bool CheckCollision(Coords<> c, Coords<> s);
    glm::vec2 vector_to_screencoords(double xpos, double ypos, const Coords<> screensize);
    void use(MergedRender* render);
    glm::mat4 projectionMatrix {1};
    //UI
    glm::mat4 UIMatrix {};
    std::unique_ptr<MergedRender> DarkBack {new MergedRender()};
};

#define END_LOOP_G END_LOOP(getwindow());

#define INIT_ARCHIVE(name) \
    SPDLOG_INFO("Start loading resources..."); \
    auto zippath = SystemAdapter::GetGameFolderName(name) + name + "_resources.zip"; \
    if (!std::filesystem::is_regular_file(zippath)){ \
        SPDLOG_WARN("archive with resources {} not found", zippath); \
        return; \
    } \
    auto archive = ZipArchive(zippath);
#define CLOSE_ARCHIVE ;


#endif //ARCADEGAMES_TILEENGINE_H
