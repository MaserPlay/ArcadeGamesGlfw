//
// Created by super on 16.06.2024.
//

#include "Texture.h"

void Texture::Load() {
    if (this->initImage > 0)
    {
        return;
    }
        // load and create a texture
        // -------------------------
        glGenTextures(1, &this->initImage);
        glBindTexture(GL_TEXTURE_2D,
                      this->initImage); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        switch (this->WrappingMode) {
            case Wrapping::NONE:
                break;
            case Wrapping::REPEAT:
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            }
            case Wrapping::MIRRORED_REPEAT:
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                break;
            }
            case Wrapping::CLAMP_TO_EDGE:
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);    // set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                break;
            }
            case Wrapping::CLAMP_TO_BORDER:
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);    // set texture wrapping to GL_REPEAT (default wrapping method)
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                break;
            }
        }
        // set texture filtering parameters
        switch (FilteringMode) {
            case Filtering::NONE:
                break;
            case Filtering::NEAREST: {
                switch (MipmapMode) {
                    case Filtering::NONE:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        break;
                    case Filtering::NEAREST:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                        break;
                    case Filtering::LINEAR:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                        break;
                }
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                break;
                }
            case Filtering::LINEAR: {
                switch (MipmapMode) {
                    case Filtering::NONE:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        break;
                    case Filtering::NEAREST:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
                        break;
                    case Filtering::LINEAR:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        break;
                }
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                break;
            }
        }
        // load image, create texture and generate mipmaps
        auto mode = GL_RGBA;
        switch (this->mode)
        {
            case Colors::RGBA:
                mode = GL_RGBA;
                break;
            case Colors::RGB:
                mode = GL_RGB;
                break;
            case Colors::ALPHA_WHITE:
                mode = GL_ALPHA;
                break;
            case Colors::RED:
                mode = GL_RED;
                break;
            default:
                mode = GL_RGBA;
                break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, mode, this->width, this->height, 0, mode,
                     GL_UNSIGNED_BYTE, this->image);
        glGenerateMipmap(GL_TEXTURE_2D);

}

Texture::~Texture() {
    glDeleteTextures(1, &this->initImage);
    delete image;
}
