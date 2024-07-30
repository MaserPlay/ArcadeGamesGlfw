//
// Created by super on 04.07.2024.
//

#include "LOG.h"
#include <fstream>
#include <memory>
#include <locale>
#include <codecvt>
#include "Font.h"
#include "main.h"
//STB
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

//MERGERD RENDER
#include "MergedRender.h"

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
//                    stbi_write_bmp((std::to_string(character) + ".bmp").c_str(), face->glyph->bitmap.width, face->glyph->bitmap.rows, 2, face->glyph->bitmap.buffer);
                    auto ch = new Char(face->glyph);
                    if (ch->getTexture()->getImage() != NULL) { m.emplace((char) character, ch); }
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
    void RenderText(const std::string& text, glm::vec2 pos, glm::mat4 mat, std::vector<std::unique_ptr<MergedRender>>& v, float size, Color color){
        if (getFont() == NULL)
        {
            return;
        }
        glm::vec2 shift = pos;
        v.resize(text.size());
        for (size_t i = 0; i < text.size(); ++i) {
            auto ch = getFont()->getChar(text[i]);
            if (ch != NULL)
            {
                shift.x += (float) ch->getBearingX() * size;
                shift.y = pos.y + (float) (ch->getHeight() - ch->getBearingY()) * -size;
                if (!v[i] || v[i]->getShaderProgram() <= 0)
                {
                    v[i] = std::make_unique<MergedRender>();
                    v[i]->setFragmentShader(MergedRender::TextureFragmentShader);
                    v[i]->quard = std::make_unique<MergedRender::Quard>(shift, ch->getWidth() * size, (float) ch->getHeight() * size, ch->getTexture());
                    v[i]->setSpeed(MergedRender::SpeedContent::DYNAMIC);
                    v[i]->load();
                } else {
                    v[i]->quard->setVertices(shift, ch->getWidth() * size, (float) ch->getHeight() * size);
                    v[i]->quard->color = color;
                    v[i]->quard->texture = ch->getTexture();
                    v[i]->VerticesChanged();
                }
                v[i]->use(mat);
                shift.x += (float) (ch->getAdvance() - (ch->getBearingX() + ch->getWidth())) / 64 * size;
            }
        }
    }
    long TextWidth(const std::string& text){
        if (getFont() == NULL)
        {
            return 0;
        }
        auto width = 0;
        for (auto& c : text) {
            if (getFont()->getChar(c) != NULL)
            {
                width += getFont()->getChar(c)->getAdvance() / 64;
            }
        }
        return width;
    }


    Char::Char(FT_GlyphSlot &glyph) : width(glyph->bitmap.width), height(glyph->bitmap.rows), bearingX(glyph->bitmap_left), bearingY(glyph->bitmap_top), advance(glyph->advance.x),
    texture(new Texture(glyph->bitmap.buffer, glyph->bitmap.width, glyph->bitmap.rows, Texture::Colors::ALPHA_WHITE, Texture::Wrapping::CLAMP_TO_EDGE, Texture::Filtering::NONE, Texture::Filtering::NONE)){
        texture->Load();
    }
}
