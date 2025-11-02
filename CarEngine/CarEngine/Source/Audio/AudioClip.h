#pragma once
#include <filesystem>
#include "soloud_wav.h"

namespace CE
{
    class AudioClip {

    friend class AudioPlayer;

    public:
        explicit AudioClip(const std::filesystem::path& audioFile,
                           bool bIsLooping = false,
                           float volume = 0.5f,
                           std::string name = "New AudioClip");

        float getLength() { return (float) audio.getLength(); }

        std::string name;

    private:
        bool bIsLooping;
        float volume;
        SoLoud::Wav audio;
    };
}