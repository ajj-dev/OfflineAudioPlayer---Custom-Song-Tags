// songlist.h
#pragma once
#include "song.h"
#include <deque>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <random>
#include <algorithm>
using json = nlohmann::json;

enum class ShuffleMode 
{
    Random,
    Genre,
    Mood,
    Weighted
};

class SongList
{
private:
    std::vector<Song> library;      // All songs
    std::deque<size_t> playQueue;   // Indices of queued songs
    size_t itemsPerPage = 25;
public:
    void addToLibrary(const Song& song);
    void addToQueue(size_t songIndex);
    void removeFromQueue(size_t queueIndex);
    std::vector<Song> getQueuePage(size_t pageNum);
    const Song& getCurrentSong(size_t currentIndex) const;
    size_t getQueueSize() const;
    void clearQueue();
    void initializeQueue();
    void updateSongTags(size_t index, const std::array<Genre, 3>& genres, const std::array<Mood, 3>& moods);
    void saveTags(const std::string& filename);
    void loadTags(const std::string& filename);
    void shuffleQueue(ShuffleMode mode, const Song& currentSong, size_t& currentIndex);
    float calculateSimilarity(const Song& song1, const Song& song2, ShuffleMode mode);

};