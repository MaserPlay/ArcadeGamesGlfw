//
// Created by super on 04.07.2024.
//

#include <zipconf.h>
#include <stb_image.h>
#include "TileEngine.h"
#include <glm/gtc/matrix_transform.hpp>

bool TileEngine::CheckCollision(Coords c, Coords s) {
    return c == s;
}

void TileEngine::size_callback(int width, int height, const Coords screensize) {
    glViewport(0,0, width, height);
    if (width >= height) {
        float Shift = (((float) width / (float) height) - ((float) screensize.x / (float) screensize.y)) * ((float) screensize.x / 2.f) - 1.f;
        projectionMatrix = glm::ortho( -Shift, (float) screensize.x * ((float) width / (float) height) - Shift, 0.f + 1.f, (float) screensize.y + 1);
    } else {
        float Shift = (((float) height / (float) width) - ((float) screensize.y / (float) screensize.x)) * ((float) screensize.y / 2.f) - 1.f;
        projectionMatrix = glm::ortho(0.f + 1.f, (float) screensize.x + 1.f, -Shift, (float) screensize.y * ((float) height / (float) width) - Shift);
    }
}

void TileEngine::use(MergedRender* render) {
    render->use(projectionMatrix);
}

glm::vec2 TileEngine::vector_to_screencoords(double xpos, double ypos, const Coords screensize) {
    if (getWidth() >= getHeight()) {
        auto Shift = (((float) getWidth() / (float) getHeight()) - ((float) screensize.x / (float) screensize.y)) * (float) (screensize.x / 2);
        return {(((xpos / getWidth())) * (screensize.x + Shift * 2) + 1) - Shift, (1 - (ypos / getHeight())) * screensize.y + 1};
    } else {
        auto Shift = (((float) getHeight() / (float) getWidth()) - ((float) screensize.y / (float) screensize.x)) * (float) (screensize.y / 2);
        return { ( (xpos / getWidth())) * screensize.x + 1,((1 - (ypos / getHeight())) * (screensize.y + Shift * 2) + 1) - Shift};
    }
}

void TileEngine::initEngine() {
    glEnable(GL_TEXTURE_2D);
//    glEnable(GL_ALPHA_TEST);
//    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
}

namespace TileEngineUtils::LoadResources
{
    void loadImage(ZipArchive* archive, const std::string &name, Texture*& texture) {
        if (texture == NULL)
        {
            SPDLOG_ERROR("render == NULL");
            return;
        }
        if (archive == NULL)
        {
            SPDLOG_ERROR("archive == NULL");
            return;
        }
        char *content = NULL; zip_uint64_t size;
        unsigned char *image = NULL; int width, height, nrChannels;
        archive->get(name, content, size);
        if (content == NULL)
        {
            SPDLOG_WARN("{} not found", name);
        } else {
            image = stbi_load_from_memory(reinterpret_cast<const unsigned char *const>(content), size, &width, &height,
                                          &nrChannels, 0);
            if (nrChannels == 4)
            {
                texture = new Texture(image, width, height);
                texture->Load();
            } else if (nrChannels == 3)
            {
                texture = new Texture(image, width, height, Texture::Colors::RGB);
                texture->Load();
            } else {
                SPDLOG_WARN("{} have {} channels. What is undefined", name, nrChannels);
            }
            stbi_image_free(content);
            SPDLOG_INFO("{} loaded", name);
        }
    }

    void loadAudio(ZipArchive* archive, const std::string &name, Sound *buffer) {
        if (buffer == NULL)
        {
            SPDLOG_ERROR("render == NULL");
            return;
        }
        if (archive == NULL)
        {
            SPDLOG_ERROR("archive == NULL");
            return;
        }
        char *content = NULL; zip_uint64_t size;
        archive->get(name, content, size);
        if (content == NULL) {
            SPDLOG_WARN("{} not found", name);
        } else {
            buffer = new Sound(content, size);
            SPDLOG_INFO("{} loaded", name);
        }
    }

    void loadFragmentShader(ZipArchive *archive, const std::string &name, MergedRender*& render) {
        if (render == NULL)
        {
            SPDLOG_ERROR("render == NULL");
            return;
        }
        if (archive == NULL)
        {
            SPDLOG_ERROR("archive == NULL");
            return;
        }
        char *content = NULL; zip_uint64_t size;
        archive->get(name, content, size);
        if (content == NULL)
        {
            SPDLOG_WARN("{} not found", name);
        } else {
            content[size] = '\0';
            render->setFragmentShader(content);
            SPDLOG_INFO("{} loaded", name);
        }
    }
    void loadVertexShader(ZipArchive *archive, const std::string &name, MergedRender *render) {
        if (render == NULL)
        {
            SPDLOG_ERROR("render == NULL");
            return;
        }
        if (archive == NULL)
        {
            SPDLOG_ERROR("archive == NULL");
            return;
        }
        char *content = NULL; zip_uint64_t size;
        archive->get(name, content, size);
        if (content == NULL)
        {
            SPDLOG_WARN("{} not found", name);
        } else {
            content[size] = '\0';
            render->setVertexShader(content);
            SPDLOG_INFO("{} loaded", name);
        }
    }
}
