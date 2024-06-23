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
        ALPHA_WHITE
    };
    Texture() = default;

    Texture(const unsigned char* image, int width, int height, Modes modes = Modes::RGBA) : isUseFiltering(false), mode(modes)
            , isUseWrapping(false), image(image), width(width), height(height), WrappingMode(GL_REPEAT), FilteringMode(GL_NEAREST), initImage(NULL), texturecords({glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)}) {
        if (image == nullptr){
            ErrorAbort("texture == NULL")
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
    bool isUseWrapping{}, isUseFiltering{};
    unsigned int initImage = 0;
    const unsigned char* image{};
    int WrappingMode{}, FilteringMode{};
    int width{}, height{};
    Modes mode = Modes::RGBA;
    std::array<glm::vec2, 4> texturecords {glm::vec2(1.0, 1.0),glm::vec2(1.0, 0.0),glm::vec2(0.0, 0.0),glm::vec2(0.0, 1.0)};

    void Load();

private:
};


#endif //ARCADEGAMES_TEXTURE_H
