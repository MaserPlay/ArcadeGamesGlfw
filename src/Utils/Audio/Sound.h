//
// Created by super on 04.07.2024.
//

#ifndef ARCADEGAMES_SOUND_H
#define ARCADEGAMES_SOUND_H


#include <al.h>
#include "Utils/System/SoundFile.hpp"

class Sound {
public:
    Sound() = default;
    Sound(const Sound &p) = delete;   // удаляем конструктор
    explicit Sound(SoundFile* file);

    explicit Sound(const std::string& filename);

    explicit Sound(const void* data, std::size_t sizeInBytes);
    void Play() const;

    virtual ~Sound();

    void Load();

private:
    //AL
    ALuint buffer = 0;
    ALuint source = 0;
    //TEMP
    std::unique_ptr<SoundFile> sf {new SoundFile()};
};


#endif //ARCADEGAMES_SOUND_H
