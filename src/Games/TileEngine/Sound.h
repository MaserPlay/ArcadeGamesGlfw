//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_SOUND_H
#define ARCADEGAMES_SOUND_H


#include <al.h>
#include "SoundFile.hpp"

class Sound {
public:
    Sound() = default;
    explicit Sound(const SoundFile* file);

    explicit Sound(const std::string& filename);

    explicit Sound(const void* data, std::size_t sizeInBytes);
    void Play();

    virtual ~Sound();

private:
    void Init(const SoundFile* sf);
    ALuint buffer = 0;
    ALuint source = 0;
};


#endif //ARCADEGAMES_SOUND_H
