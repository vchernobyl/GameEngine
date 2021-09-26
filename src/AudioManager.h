#pragma once

#include <SDL/SDL_mixer.h>
#include <string>
#include <map>

class SoundEffect {
public:
    void Play(int loops = 0);
private:
    friend class AudioManager;
    Mix_Chunk* chunk = nullptr;
};

class Music {
public:
    void Play(int times = 1);
    void Pause();
    void Stop();
    void Resume();
private:
    friend class AudioManager;
    Mix_Music* music = nullptr;
};

class AudioManager {
public:
    void Init();
    void Destroy();

    SoundEffect LoadSoundEffect(const std::string& filePath);
    Music LoadMusic(const std::string& filePath);

private:
    bool isInitialized = false;

    std::map<std::string, Mix_Chunk*> effectCache;
    std::map<std::string, Mix_Music*> musicCache;
};
