//
// Created by super on 04.07.2024.
//

#include "LOG.h"
#include <fstream>
#include "Font.h"
#include "main.h"
//STB
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

FT_Library ft = NULL;
namespace Font{
    void InitLib() {
        if (FT_Init_FreeType(&ft))
        {
            SPDLOG_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
        }
    }
    void Destroy(){
        FT_Done_FreeType(ft); // Завершение работы FreeType
    }

    Font::Font(const FT_Byte* file_base, FT_Long file_size) {
        if (ft != NULL) {
            if (FT_New_Memory_Face(ft, file_base, file_size, 0, &face)) {
                SPDLOG_WARN("ERROR::FREETYPE: Failed to load font");
                return;
            }
        }
        Init();
    }

    Font::Font(const std::string& filename) {
        if (ft != NULL) {
            if (FT_New_Face(ft, filename.c_str(), 0, &face)) {
                SPDLOG_WARN("ERROR::FREETYPE: Failed to load font");
                return;
            }
        }
        Init();
    }

    void Font::Init() {
        if (ft != NULL)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
            FT_Set_Pixel_Sizes(face, 0, 48);
            FT_UInt index;
            FT_ULong character = FT_Get_First_Char(face, &index);

            while (true) {
                // to do something
                character = FT_Get_Next_Char(face, character, &index);

                if (FT_Load_Char(face, character, FT_LOAD_RENDER))
                {
                    SPDLOG_WARN("ERROR::FREETYTPE: Failed to load Glyph {}", (char) character);
                } else {
                    auto ch = new Char(face->glyph);
                    if (ch->getTexture()->getImage() != NULL) { m.insert({(char) character, ch}); }
                }

                if (!index) break; // if FT_Get_Next_Char write 0 to index then
                // have no more characters in font face
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 0); // Disable byte-alignment restriction
        }
    }

    Font::~Font() {
        FT_Done_Face(face);   // Завершение работы с шрифтом face
    }

    void RenderText(const std::string& text, glm::vec2 pos, float size){
        if (getFont() == NULL)
        {
            return;
        }
        glm::vec2 shift = pos;
        for (auto& c :text) {
            auto ch = getFont()->getChar(c);
            shift.x += (float) ch->getBearingX() * size;
            shift.y = pos.y + (float) (ch->getHeight() - ch->getBearingY()) * size;
            glColor4d(1.,1.,1.,1.);
            glBindTexture(GL_TEXTURE_2D, ch->getTexture()->getInitImage());
            glBegin(GL_QUADS);
            glTexCoord2d(1.0, 1.0);
            glVertex2f(shift.x + (float) ch->getWidth() * size,shift.y);
            glTexCoord2d(0.0, 1.0);
            glVertex2f(shift.x,shift.y);
            glTexCoord2d(0.0, 0.0);
            glVertex2f(shift.x,shift.y + (float) ch->getHeight() * size);
            glTexCoord2d(1.0, 0.0);
            glVertex2f(shift.x + (float) ch->getWidth() * size,shift.y + (float) ch->getHeight() * size);
            glEnd();
            shift.x += (float) (ch->getAdvance() - (ch->getBearingX() + ch->getWidth())) / 64 * size;
        }
    }


    Char::Char(FT_GlyphSlot &glyph) : width(glyph->bitmap.width), height(glyph->bitmap.rows), bearingX(glyph->bitmap_left), bearingY(glyph->bitmap_top), advance(glyph->advance.x),
    texture(new Texture(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, Texture::Modes::ALPHA_WHITE)){
        texture->Load();
    }
}
