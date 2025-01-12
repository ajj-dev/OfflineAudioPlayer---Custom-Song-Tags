#include "SongList.h"

void SongList::addToQueue(size_t songIndex)
{
    if (songIndex < library.size())
    {
        playQueue.push_back(songIndex);
    }
}

void SongList::removeFromQueue(size_t queueIndex)
{
    if (queueIndex < playQueue.size())
    {
        playQueue.erase(playQueue.begin() + queueIndex);
    }
}

std::vector<Song> SongList::getQueuePage(size_t pageNum)
{
    std::vector<Song> page;
    size_t startIdx = pageNum * itemsPerPage;
    size_t endIdx = std::min(startIdx + itemsPerPage, playQueue.size());

    for (size_t i = startIdx; i < endIdx; i++)
    {
        page.push_back(library[playQueue[i]]);
    }
    return page;
}

void SongList::addToLibrary(const Song& song)
{
    library.push_back(song);
}

const Song& SongList::getCurrentSong(size_t currentIndex) const
{
    return library[playQueue[currentIndex]];
}

size_t SongList::getQueueSize() const
{
    return playQueue.size();
}

void SongList::clearQueue()
{
    playQueue.clear();
}

void SongList::initializeQueue()
{
    playQueue.clear();
    for (size_t i = 0; i < library.size(); i++)
    {
        playQueue.push_back(i);
    }
}

void SongList::updateSongTags(size_t index, const std::array<Genre, 3>& genres, const std::array<Mood, 3>& moods)
{
    if (index < library.size()) 
    {
        library[index].setGenres(genres);
        library[index].setMoods(moods);
        // Save to JSON here or queue for saving
        saveTags("songTags.json");
    }
}

void SongList::saveTags(const std::string& filename) 
{
    json j = json::object();
    for (const auto& song : library)
    {
        json songJson;
        songJson["genres"] = 
        {
            static_cast<int>(song.getGenres()[0]),
            static_cast<int>(song.getGenres()[1]),
            static_cast<int>(song.getGenres()[2])
        };
        songJson["moods"] = 
        {
            static_cast<int>(song.getMoods()[0]),
            static_cast<int>(song.getMoods()[1]),
            static_cast<int>(song.getMoods()[2])
        };
        j[song.getFilePath()] = songJson;
    }

    std::ofstream o(filename);
    o << std::setw(4) << j << std::endl;
}

void SongList::loadTags(const std::string& filename) 
{
    if (!std::filesystem::exists(filename)) return;

    std::ifstream i(filename);
    json j = json::parse(i);

    for (auto& song : library) 
    {
        if (j.contains(song.getFilePath())) {
            auto& songJson = j[song.getFilePath()];
            std::array<Genre, 3> genres = {
                static_cast<Genre>(songJson["genres"][0]),
                static_cast<Genre>(songJson["genres"][1]),
                static_cast<Genre>(songJson["genres"][2])
            };
            std::array<Mood, 3> moods = {
                static_cast<Mood>(songJson["moods"][0]),
                static_cast<Mood>(songJson["moods"][1]),
                static_cast<Mood>(songJson["moods"][2])
            };
            song.setGenres(genres);
            song.setMoods(moods);
        }
    }
}

void SongList::shuffleQueue(ShuffleMode mode, const Song& currentSong, size_t& currentIndex)
{
    // Save current song index
    size_t currentLibraryIndex = playQueue[currentIndex];
    // Remove current song from queue temporarily
    playQueue.erase(playQueue.begin() + currentIndex);

    // Shuffle/sort remaining songs
    if (mode == ShuffleMode::Random)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(playQueue.begin(), playQueue.end(), g);
    }
    else 
    {
        std::sort(playQueue.begin(), playQueue.end(),
            [this, &currentSong, mode](size_t a, size_t b)
            {
                return calculateSimilarity(library[a], currentSong, mode) >
                    calculateSimilarity(library[b], currentSong, mode);
            });
    }

    // Reinsert current song at beginning and update current index
    playQueue.insert(playQueue.begin(), currentLibraryIndex);
    currentIndex = 0;  // Current song is now at the start of the queue
}

float SongList::calculateSimilarity(const Song& song1, const Song& song2, ShuffleMode mode) 
{
    float similarity = 0.0f;

    if (mode == ShuffleMode::Genre or mode == ShuffleMode::Weighted) {
        // Count matching genres
        for (auto g1 : song1.getGenres()) 
        {
            for (auto g2 : song2.getGenres()) 
            {
                if (g1 == g2 and g1 != Genre::None) similarity += 1.0f;
            }
        }
    }

    if (mode == ShuffleMode::Mood or mode == ShuffleMode::Weighted) 
    {
        // Count matching moods
        for (auto m1 : song1.getMoods()) 
        {
            for (auto m2 : song2.getMoods()) 
            {
                if (m1 == m2 and m1 != Mood::None) similarity += 1.0f;
            }
        }
    }

    return similarity;
}