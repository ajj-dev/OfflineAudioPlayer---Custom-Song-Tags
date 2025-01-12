#pragma once
#include "miniaudio.h"
#include "songlist.h"
#include "imgui.h"
#include <memory>
#include <iostream>
#include <filesystem>

class MusicPlayer
{
public:
    MusicPlayer();
    ~MusicPlayer();

    void initialize();
    void update();
    void render();

    // Audio control functions
    void setVolume(float volume);
    float getPlaybackPosition();
    void setPlaybackPosition(float position);
    float getDuration();

    void play();
    void pause();
    void next();
    void previous();

private:
    void loadSongsFromDirectory(const std::string& directoryPath);
    ShuffleMode currentShuffleMode = ShuffleMode::Random;
    void displayShuffleControls();
  
    const std::string songDirectory = "songLibrary";
    SongList playlist;
    size_t currentSongIndex;
    bool isPlaying;
    bool isPaused;
    ma_engine engine;
    ma_sound currentSound;
    ma_uint64 sliderChangedValue = 0;
    float volume = 1.0f;
};