#pragma once
#include <unordered_map>
#include "soloud.h"
#include "AudioClip.h"

namespace CE
{
     class AudioPlayer {

     public:
         AudioPlayer();

         AudioPlayer(const AudioPlayer& other) = delete;

         AudioPlayer(AudioPlayer&& other) noexcept {
             std::swap(this->audioSystem, other.audioSystem);
             std::swap(this->audioClips, other.audioClips);
         }

         ~AudioPlayer();

         void playClip(AudioClip* audioClip);
         void pauseClip(AudioClip* audioClip);
         void stopClip(AudioClip* audioClip);

         void playAllPausedClips();
         void pauseAllClips();
         void stopAllClips();

         float getClipVolume(AudioClip* audioClip);
         void setClipVolume(AudioClip* audioClip, float volume);

         float getGlobalVolume();
         void setGlobalVolume(float volume);

         bool isClipPlaying(AudioClip* audioClip);

         bool isClipLooping(AudioClip* audioClip);
         void setClipLooping(AudioClip* audioClip, bool bIsLooping);

         void fadeClip(AudioClip* audioClip, float targetVolume, float seconds);

         void pauseClipDelayed(AudioClip* audioClip, float seconds);
         void stopClipDelayed(AudioClip* audioClip, float seconds);

     private:
         SoLoud::Soloud audioSystem;
         std::unordered_map<AudioClip*, SoLoud::handle> audioClips;
     };
}