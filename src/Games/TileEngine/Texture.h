//
// Created by super on 16.06.2024.
//

#ifndef ARCADEGAMES_TEXTURE_H
#define ARCADEGAMES_TEXTURE_H


#include <glm/vec2.hpp>
#include "GL/glew.h"
#include "array"
#include "debug.h"

class Texture final {
public:
    enum class Colors{
        RGBA,
        RGB,
        ALPHA_WHITE,
        RED
    };
    enum class Wrapping{
        NONE,
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };
    enum class Filtering{
        NONE,
        NEAREST,
        LINEAR
    };
    Texture() = default;

    Texture(const unsigned char* image, int width, int height, Colors modes = Colors::RGBA, Wrapping WrappingMode = Wrapping::REPEAT, Filtering FilteringMode = Filtering::LINEAR, Filtering MipmapMode = Filtering::LINEAR) : isUseFiltering(false), mode(modes)
            , isUseWrapping(false), image(image), MipmapMode(MipmapMode), width(width), height(height), FilteringMode(FilteringMode), WrappingMode(WrappingMode), initImage(NULL), texturecords({glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)}) {
        if (image == nullptr){
            SPDLOG_WARN("texture == NULL");
        }
    }
    std::array<glm::vec2, 4> texturecords {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};

    virtual ~Texture();

    void Load();

    [[nodiscard]] int getWidth() const {
        return width;
    }

    [[nodiscard]] int getHeight() const {
        return height;
    }

    [[nodiscard]] unsigned int getInitImage() const {
        return initImage;
    }

    [[nodiscard]] Colors getMode() const {
        return mode;
    }

    [[nodiscard]] const unsigned char *getImage() const {
        return image;
    }

    [[nodiscard]] Wrapping getWrappingMode() const {
        return WrappingMode;
    }

    [[nodiscard]] Filtering getFilteringMode() const {
        return FilteringMode;
    }

    [[nodiscard]] bool isUseWrapping1() const {
        return isUseWrapping;
    }

    [[nodiscard]] bool isUseFiltering1() const {
        return isUseFiltering;
    }

private:
    int width{}, height{};
    unsigned int initImage = 0;
    Colors mode = Colors::RGBA;
    const unsigned char* image{};
    Wrapping WrappingMode = Wrapping::REPEAT;
    Filtering FilteringMode = Filtering::LINEAR;
    Filtering MipmapMode = Filtering::LINEAR;
    bool isUseWrapping{}, isUseFiltering{};
};


#endif //ARCADEGAMES_TEXTURE_H
