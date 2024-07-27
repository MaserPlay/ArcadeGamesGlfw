//
// Created by super on 04.07.2024.
//

#include <spdlog/spdlog.h>
#include "Sound.h"

Sound::Sound(const std::string &filename) {
    auto sf = new SoundFile();
    sf->openRead(filename);
    Init(sf);
    delete sf;
}

Sound::Sound(const void *data, std::size_t sizeInBytes) {
    auto sf = new SoundFile();
    sf->openRead(data, sizeInBytes);
    Init(sf);
    delete sf;
}

Sound::Sound(const SoundFile *file) {
    Init(file);
}

void Sound::Init(const SoundFile *sf) {
    ALenum format;
    if (sf->getChannelCount() == 1)
        format = AL_FORMAT_MONO16;
    else if (sf->getChannelCount() == 2)
        format = AL_FORMAT_STEREO16;
    else {
        spdlog::error("ERROR: unrecognised wave format: {} channels", sf->getChannelCount());
        return;
    }
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, sf->getOpenAlData(), sf->getOpenAlDataSize(), sf->getSampleRate());
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
}

void Sound::Play() {
    if (source != 0) {
        alSourcePlay(source);

        ALint state = AL_PLAYING;

        while (state == AL_PLAYING) {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        }
    }
}

Sound::~Sound() {
    if (source != 0) {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
    }
}
