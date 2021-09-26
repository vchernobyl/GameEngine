#include "AudioManager.h"
#include "Errors.h"

void SoundEffect::Play(int loops) {
    if (Mix_PlayChannel(-1, chunk, loops) == -1) {
	if (Mix_PlayChannel(0, chunk, loops) == -1) {
	    FatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
	}
    }
}

void Music::Play(int times) {
    if (Mix_PlayMusic(music, times) == -1) {
	FatalError("Mix_PlayMusic error: " + std::string(Mix_GetError()));
    }
}

void Music::Pause() {
    Mix_PauseMusic();
}

void Music::Stop() {
    Mix_HaltMusic();
}

void Music::Resume() {
    Mix_ResumeMusic();
}

void AudioManager::Init() {
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1) {
	FatalError("Mix_Init error: " + std::string(Mix_GetError()));
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
	FatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
    }

    isInitialized = true;
}

void AudioManager::Destroy() {
    if (isInitialized) {
	isInitialized = false;
	Mix_Quit();
    }
}

SoundEffect AudioManager::LoadSoundEffect(const std::string& filePath) {
    auto it = effectCache.find(filePath);
    SoundEffect effect;
    
    if (it == effectCache.end()) {
	Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
	if (!chunk) {
	    FatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
	}
	effectCache[filePath] = chunk;
	effect.chunk = chunk;
    } else {
	effect.chunk = it->second;
    }

    return effect;
}

Music AudioManager::LoadMusic(const std::string& filePath) {
    auto it = musicCache.find(filePath);
    Music music;
    
    if (it == musicCache.end()) {
	Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
	if (!mixMusic) {
	    FatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
	}
	musicCache[filePath] = mixMusic;
	music.music = mixMusic;
    } else {
	music.music = it->second;
    }

    return music;
}
