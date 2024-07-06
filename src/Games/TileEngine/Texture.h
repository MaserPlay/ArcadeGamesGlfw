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
    enum class Modes{
        RGBA,
        RGB,
        ALPHA_WHITE,
        RED
    };
    Texture() = default;

    Texture(const unsigned char* image, int width, int height, Modes modes = Modes::RGBA) : isUseFiltering(false), mode(modes)
            , isUseWrapping(false), image(image), width(width), height(height), WrappingMode(GL_REPEAT), FilteringMode(GL_NEAREST), initImage(NULL), texturecords({glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)}) {
        if (image == nullptr){
            SPDLOG_WARN("texture == NULL");
        }
    }
   // explicit Texture(wxImage* image, Modes modes = Modes::RGBA) : Texture(image->GetData(),image->GetWidth(),image->GetHeight(), modes){}

    Texture* useWrapping(int mode = GL_REPEAT)
    {
        WrappingMode = mode;
        return this;
    }
    Texture* useFiltering(int mode = GL_NEAREST)
    {
        FilteringMode = mode;
        return this;
    }
    std::array<glm::vec2, 4> texturecords {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};

    virtual ~Texture();

    void Load();

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    unsigned int getInitImage() const {
        return initImage;
    }

    Modes getMode() const {
        return mode;
    }

    const unsigned char *getImage() const {
        return image;
    }

    int getWrappingMode() const {
        return WrappingMode;
    }

    int getFilteringMode() const {
        return FilteringMode;
    }

    bool isUseWrapping1() const {
        return isUseWrapping;
    }

    bool isUseFiltering1() const {
        return isUseFiltering;
    }

private:
    int width{}, height{};
    unsigned int initImage = 0;
    Modes mode = Modes::RGBA;
    const unsigned char* image{};
    int WrappingMode{}, FilteringMode{};
    bool isUseWrapping{}, isUseFiltering{};
};


#endif //ARCADEGAMES_TEXTURE_H
