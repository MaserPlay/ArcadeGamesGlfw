//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_FONT_H
#define ARCADEGAMES_FONT_H
#include <ft2build.h>
#include FT_FREETYPE_H
//#include FT_GLYPH_H
#include "Texture.h"


#include <cstddef>
#include <map>

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
    void RenderText(const std::string& text, glm::vec2 pos, float size);
    class Font final {
    public:
        virtual ~Font();

        Font() = default;

        Font(const FT_Byte* file_base, FT_Long file_size);

        explicit Font(const std::string& filename);

        [[nodiscard]] const Char* getChar(char ch) const{
            return m.at(ch);
        }
//        void Load()
//        {
//            for (auto& c:m) {
//                c.second->Load();
//            }
//        }

    private:
        void Init();

        FT_Face face = nullptr;
        std::map<char, Char*> m {};
    };
}


#endif //ARCADEGAMES_FONT_H
