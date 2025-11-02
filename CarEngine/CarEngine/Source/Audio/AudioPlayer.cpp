#include "AudioPlayer.h"

namespace CE
{
    AudioPlayer::AudioPlayer()
    {
        audioSystem.init();
    }

    void AudioPlayer::playClip(AudioClip* audioClip)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
        {
            audioSystem.setPause(audioClips.at(audioClip), false);
        }
        else
        {
            auto audioClipHandle = audioSystem.play(audioClip->audio);
            audioClips.insert({audioClip, audioClipHandle});
        }
    }

    void AudioPlayer::pauseClip(AudioClip* audioClip)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
            audioSystem.setPause(audioClips.at(audioClip), true);
    }

    void AudioPlayer::stopClip(AudioClip* audioClip)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
        {
            audioSystem.stop(audioClips.at(audioClip));
            audioClips.erase(audioClip);
        }
    }

    void AudioPlayer::playAllPausedClips()
    {
        audioSystem.setPauseAll(false);
    }

    void AudioPlayer::pauseAllClips()
    {
        audioSystem.setPauseAll(true);
    }

    void AudioPlayer::stopAllClips()
    {
        audioSystem.stopAll();
    }

    float AudioPlayer::getClipVolume(AudioClip* audioClip)
    {
        if (!audioClip)
            return 0.f;

        if (audioClips.find(audioClip) != audioClips.end())
            return audioSystem.getVolume(audioClips.at(audioClip));
        else
            return audioClip->volume;
    }

    void AudioPlayer::setClipVolume(AudioClip* audioClip, float volume)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
        {
            audioSystem.setVolume(audioClips.at(audioClip), volume);
        }
        else
        {
            audioClip->audio.setVolume(volume);
            audioClip->volume = volume;
        }
    }

    float AudioPlayer::getGlobalVolume()
    {
        return audioSystem.getGlobalVolume();
    }

    void AudioPlayer::setGlobalVolume(float volume)
    {
        audioSystem.setGlobalVolume(volume);
    }

    bool AudioPlayer::isClipPlaying(AudioClip* audioClip)
    {
        if (!audioClip)
            return false;

        if (audioClips.find(audioClip) != audioClips.end())
            return !audioSystem.getPause(audioClips.at(audioClip));

        return false;
    }

    bool AudioPlayer::isClipLooping(AudioClip* audioClip)
    {
        if (!audioClip)
            return false;

        if (audioClips.find(audioClip) != audioClips.end())
            return audioSystem.getLooping(audioClips.at(audioClip));
        else
            return audioClip->bIsLooping;
    }

    void AudioPlayer::setClipLooping(AudioClip* audioClip, bool bIsLooping)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
        {
            audioSystem.setLooping(audioClips.at(audioClip), bIsLooping);
        }
        else
        {
            audioClip->audio.setLooping(bIsLooping);
            audioClip->bIsLooping = bIsLooping;
        }
    }

    void AudioPlayer::fadeClip(AudioClip* audioClip, float targetVolume, float seconds)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
            audioSystem.fadeVolume(audioClips.at(audioClip), targetVolume, seconds);
    }

    void AudioPlayer::pauseClipDelayed(AudioClip* audioClip, float seconds)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
            audioSystem.schedulePause(audioClips.at(audioClip), seconds);
    }

    void AudioPlayer::stopClipDelayed(AudioClip* audioClip, float seconds)
    {
        if (!audioClip)
            return;

        if (audioClips.find(audioClip) != audioClips.end())
            audioSystem.scheduleStop(audioClips.at(audioClip), seconds);
    }

    AudioPlayer::~AudioPlayer()
    {
        audioSystem.deinit();
    }
}