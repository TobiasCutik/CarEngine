#include "AudioClip.h"
#include <utility>

namespace CE
{
    AudioClip::AudioClip(const std::filesystem::path& audioFile, bool bIsLooping, float volume, std::string name) : name(std::move(name)), bIsLooping(bIsLooping), volume(volume)
    {
        audio.load(audioFile.string().c_str());
        audio.setVolume(volume);
        audio.setLooping(bIsLooping);
    }
}