//
// Created by super on 16.06.2024.
//

#include "Texture.h"

void Texture::Load() {
        // load and create a texture
        // -------------------------
        glGenTextures(1, &this->initImage);
        glBindTexture(GL_TEXTURE_2D,
                      this->initImage); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                        this->WrappingMode);    // set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrappingMode);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        auto mode = GL_RGBA;
        switch (this->mode)
        {
            case Modes::RGBA:
                mode = GL_RGBA;
                break;
            case Modes::RGB:
                mode = GL_RGB;
                break;
            case Modes::ALPHA_WHITE:
                mode = GL_ALPHA;
                break;
            default:
                mode = GL_RGBA;
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, mode, this->width, this->height, 0, mode,
                     GL_UNSIGNED_BYTE, this->image);
        glGenerateMipmap(GL_TEXTURE_2D);

}
