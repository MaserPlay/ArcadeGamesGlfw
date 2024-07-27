//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_FONT_H
#define ARCADEGAMES_FONT_H
#include "Other.h"
#include <cstddef>
#include <map>
#include "Texture.h"
#include "glm/mat4x4.hpp"

//FREE TYPE
#include <ft2build.h>
#include "freetype/freetype.h"
#include FT_GLYPH_H
#include FT_FREETYPE_H

//GLM
#include "glm/vec4.hpp"

//CLASS
class MergedRender;


namespace Font {
    class Char final {
    public:
//        Char() = default;
        explicit Char(FT_GlyphSlot &glypth);


        void Load(){texture->Load();}

        [[nodiscard]] Texture *getTexture() const {
            return texture;
        }

        ///ширина глифа в пикселях
        [[nodiscard]] unsigned int getWidth() const {
            return width;
        }

        ///высота глифа в пикселях
        [[nodiscard]] unsigned int getHeight() const {
            return height;
        }

        ///горизонтальное смещение верхней левой точки глифа по отношению к origin
        [[nodiscard]] int getBearingX() const {
            return bearingX;
        }

        ///вертикальное смещение верхней левой точки глифа по отношению к origin
        [[nodiscard]] int getBearingY() const {
            return bearingY;
        }

        /// горизонтальное смещение начала следующего глифа в 1/64 пикселях по отношению к origin
        [[nodiscard]] long getAdvance() const {
            return advance;
        }

    private:
        Texture* texture = new Texture();
        ///ширина глифа в пикселях
        unsigned int width;
        ///высота глифа в пикселях
        unsigned int height;
        ///горизонтальное смещение верхней левой точки глифа по отношению к origin
        int bearingX;
        ///вертикальное смещение верхней левой точки глифа по отношению к origin
        int bearingY;
        /// горизонтальное смещение начала следующего глифа в 1/64 пикселях по отношению к origin
        long advance;
    };
    void InitLib();
    void Destroy();
    void RenderText(const std::string& text, glm::vec2 pos, glm::mat4 mat, std::vector<std::unique_ptr<MergedRender>>& v, float size = 1, Color color = {1., 1., 1., 1.});
    long TextWidth(const std::string& text);
    class Font final {
    public:
        virtual ~Font();

        Font() = default;

        Font(const FT_Byte* file_base, FT_Long file_size);

        explicit Font(const std::string& filename);

        [[nodiscard]] const Char* getChar(char ch) const{
            try { return m.at(ch); } catch (...) {return NULL;}
        }

    private:
        void Init();

        FT_Face face = nullptr;
        std::map<char, Char*> m {};
    };
}


#endif //ARCADEGAMES_FONT_H
