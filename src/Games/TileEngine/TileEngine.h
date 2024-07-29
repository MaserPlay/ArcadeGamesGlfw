//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_TILEENGINE_H
#define ARCADEGAMES_TILEENGINE_H


#include "Utils/BaseGame.h"
#include "Other.h"
#include "Utils/Render/Texture.h"
#include "Utils/Audio/Sound.h"
#include "glm/vec4.hpp"
#include "Utils/System/ZipArchive.h"
#include "Utils/Render/MergedRender.h"

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
public:
    static void initEngine();
};
#define START_LOOP \
                    glfwPollEvents(); \
                    glClearColor(0.f, 0.f, 0.f, 1.0f); \
                    glClear(GL_COLOR_BUFFER_BIT); \

#define END_LOOP \
                glfwSwapBuffers(getwindow());

namespace TileEngineUtils::LoadResources{
    void loadImage(ZipArchive& archive, const std::string& name, std::shared_ptr<Texture>& texture);
    void loadAudio(ZipArchive& archive, const std::string& name, std::unique_ptr<Sound>& buffer);
    void loadVertexShader(ZipArchive& archive, const std::string& name, std::unique_ptr<MergedRender>& render);
    void loadFragmentShader(ZipArchive& archive, const std::string& name, std::unique_ptr<MergedRender>& render);

#define INIT_ARCHIVE(name) \
    SPDLOG_INFO("Start loading resources..."); \
    auto zippath = SystemAdapter::GetGameFolderName(name) + name + "_resources.zip"; \
    if (!std::filesystem::is_regular_file(zippath)){ \
        SPDLOG_WARN("archive with resources {} not found", zippath); \
        return; \
    } \
    auto archive = ZipArchive(zippath);
#define CLOSE_ARCHIVE ;
}

#endif //ARCADEGAMES_TILEENGINE_H
