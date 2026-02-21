#include "MusicPlayer.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

MusicPlayer::MusicPlayer() : currentSongIndex(0), isPlaying(false), isPaused(false)
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio engine!";
    }
}

MusicPlayer::~MusicPlayer()
{
    ma_sound_uninit(&currentSound);
    ma_engine_uninit(&engine);
    playlist.saveTags("songTags.json");
}

void MusicPlayer::initialize()
{
    loadSongsFromDirectory(songDirectory);
    playlist.initializeQueue();
    playlist.loadTags("songTags.json");
}

void MusicPlayer::update()
{
    if (isPlaying and !isPaused and (getPlaybackPosition() / getDuration()) >= 1.0f)
    {
        next();
    }
}

void MusicPlayer::render()
{
    ImGui::Begin("Music Player");

    // Current song info
    if (playlist.getQueueSize() != 0)
    {
        const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
        std::string songInfo = (currentSong.getTitle() == "Unknown" && currentSong.getArtist() == "Unknown")
            ? std::filesystem::path(currentSong.getFilePath()).filename().string()
            : currentSong.getTitle() + " - " + currentSong.getArtist();
        ImGui::Text("%s", songInfo.c_str());
    }

    // Playback controls
    if (ImGui::Button(isPlaying ? "Pause" : "Play"))
    {
        if (isPlaying) pause();
        else play();
    }

    ImGui::SameLine();
    if (ImGui::Button("Previous")) previous();

    ImGui::SameLine();
    if (ImGui::Button("Next")) next();

    // Progress bar
    float position = getPlaybackPosition();
    float duration = getDuration();
    if (ImGui::SliderFloat("##progress", &position, 0.0f, duration, "%.2f"))
    {
        ma_uint64 sampleRate = ma_engine_get_sample_rate(&engine);
        sliderChangedValue = static_cast<ma_uint64>(position * sampleRate);
    }

    if (ImGui::IsItemDeactivated())
    {
        ma_result result = ma_sound_seek_to_pcm_frame(&currentSound, sliderChangedValue);
        if (result != MA_SUCCESS)
        {
            std::cerr << "Failed to seek in audio file. Error code:" << result;
        }
    }

    // Volume
    if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f))
    {
        setVolume(volume);
    }

    //Shuffle Control
    displayShuffleControls();

    // Queue display with pagination
    static int currentPage = 0;
    std::vector<Song> currentPageSongs = playlist.getQueuePage(currentPage);

    if (ImGui::BeginChild("Queue", ImVec2(0, 200), true))
    {
        for (size_t i = 0; i < currentPageSongs.size(); i++)
        {
            const Song& song = currentPageSongs[i];
            std::string songInfo = (song.getTitle() == "Unknown" and song.getArtist() == "Unknown")
                ? std::filesystem::path(song.getFilePath()).filename().string()
                : song.getTitle() + " - " + song.getArtist();

            if (ImGui::Selectable(songInfo.c_str(), i == currentSongIndex))
            {
                currentSongIndex = i + (currentPage * 25);
               
                ma_sound_stop(&currentSound);
                ma_sound_uninit(&currentSound);
                const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
                ma_result result = ma_sound_init_from_file(&engine, currentSong.getFilePath().c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &currentSound);
                if (result != MA_SUCCESS)
                {
                    std::cout << "Failed to initialize sound! Error code: " << result << std::endl;
                    return;
                }
                if (!isPaused)
                {
                    result = ma_sound_start(&currentSound);
                    if (result != MA_SUCCESS)
                    {
                        std::cout << "Failed to start sound! Error code:" << result;
                        ma_sound_uninit(&currentSound);
                        return;
                    }
                    ma_sound_set_volume(&currentSound, volume);
                    isPlaying = true;
                }
            }
            // Song Tags Popup Menu
            if (ImGui::BeginPopupContextItem(("context" + std::to_string(i)).c_str()))
            {
                ImGui::Text("Edit Tags for: %s", songInfo.c_str());
                ImGui::Separator();

                std::array<Genre, 3> genres = song.getGenres();
                for (int j = 0; j < 3; j++)
                {
                    if (ImGui::BeginCombo(("Genre " + std::to_string(j + 1)).c_str(),
                        genreToString(genres[j]).c_str()))
                    {
                        for (int g = 0; g < static_cast<int>(Genre::COUNT); g++)
                        {
                            bool selected = (genres[j] == static_cast<Genre>(g));
                            if (ImGui::Selectable(genreToString(static_cast<Genre>(g)).c_str(), selected))
                            {
                                genres[j] = static_cast<Genre>(g);
                                size_t actualIndex = i + (currentPage * 25);
                                playlist.updateSongTags(actualIndex, genres, song.getMoods());
                            }
                        }
                        ImGui::EndCombo();
                    }
                }

                std::array<Mood, 3> moods = song.getMoods();
                for (int j = 0; j < 3; j++)
                {
                    if (ImGui::BeginCombo(("Mood " + std::to_string(j + 1)).c_str(),
                        moodToString(moods[j]).c_str()))
                    {
                        for (int m = 0; m < static_cast<int>(Mood::COUNT); m++)
                        {
                            bool selected = (moods[j] == static_cast<Mood>(m));
                            if (ImGui::Selectable(moodToString(static_cast<Mood>(m)).c_str(), selected))
                            {
                                moods[j] = static_cast<Mood>(m);
                                size_t actualIndex = i + (currentPage * 25);
                                playlist.updateSongTags(actualIndex, song.getGenres(), moods);
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::EndChild();

    // Page controls
    if (ImGui::Button("Previous Page") and currentPage > 0)
        currentPage--;

    ImGui::SameLine();
    if (ImGui::Button("Next Page") and (currentPage + 1) * 25 < playlist.getQueueSize())
        currentPage++;

    ImGui::End();
}

void MusicPlayer::play()
{
    if (!isPaused)
    {
        if (playlist.getQueueSize() != 0)
        {
            const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
            ma_result result1 = ma_sound_init_from_file(&engine, currentSong.getFilePath().c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &currentSound);
            if (result1 != MA_SUCCESS)
            {
                std::cout << "Failed to initialize sound! Error code:" << result1;
                return;
            }

            result1 = ma_sound_start(&currentSound);
            if (result1 != MA_SUCCESS)
            {
                isPlaying = false;
                std::cout << "Failed to start sound! Error code:" << result1;
                ma_sound_uninit(&currentSound);
                return;
            }
            ma_sound_set_volume(&currentSound, volume);
            isPlaying = true;
        }
    }
    else
    {
        ma_result result = ma_sound_start(&currentSound);
        if (result != MA_SUCCESS)
        {
            std::cout << "Failed to start sound! Error code:" << result;
            ma_sound_uninit(&currentSound);
            return;
        }
        ma_sound_set_volume(&currentSound, volume);
        isPlaying = true;
        isPaused = false;
    }
}

void MusicPlayer::pause()
{
    if (isPlaying)
    {
        ma_result result = ma_sound_stop(&currentSound);
        if (result != MA_SUCCESS)
        {
            std::cout << "Failed to stop sound! Error code:" << result;
            return;
        }
        isPlaying = false;
        isPaused = true;
    }
}

void MusicPlayer::next()
{
    if (playlist.getQueueSize() == 0)
    {
        std::cout << "No song in the queue to continue to." << std::endl;
        return;
    }

    // Move to the next song or loop back
    if (currentSongIndex + 1 >= playlist.getQueueSize())
    {
        currentSongIndex = 0; // Loop back to the start
    }
    else
    {
        ++currentSongIndex; // Move to the next song
    }

    // Stop and clean up the current sound
    ma_sound_uninit(&currentSound);

    // Initialize the new sound
    const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
    ma_result result = ma_sound_init_from_file(&engine, currentSong.getFilePath().c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &currentSound);
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to initialize sound! Error code: " << result << std::endl;
        return;
    }

    // Start playback if not paused
    if (!isPaused)
    {
        result = ma_sound_start(&currentSound);
        if (result != MA_SUCCESS)
        {
            isPlaying = false;
            std::cout << "Failed to start sound! Error code: " << result << std::endl;
            ma_sound_uninit(&currentSound);
            return;
        }
        ma_sound_set_volume(&currentSound, volume);
        isPlaying = true;
    }
}

void MusicPlayer::previous()
{
    if (playlist.getQueueSize() == 0)
    {
        std::cout << "No song in the playlist to play." << std::endl;
        return;
    }

    bool restartCurrentSong = true;

    // Check if playback is within the first 3 seconds of the current song
    if (getPlaybackPosition() <= 3.0f)
    {
        restartCurrentSong = false;

        // Move to the previous song or loop to the last song
        if (currentSongIndex == 0)
        {
            currentSongIndex = playlist.getQueueSize() - 1; // Loop back to the last song
        }
        else
        {
            --currentSongIndex; // Go to the previous song
        }
    }

    // Stop and clean up the current sound
    ma_sound_uninit(&currentSound);

    // Initialize the appropriate song
    const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
    ma_result result = ma_sound_init_from_file(&engine, currentSong.getFilePath().c_str(), MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &currentSound);
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to initialize sound! Error code: " << result << std::endl;
        return;
    }

    // Start playback if not paused
    if (!isPaused)
    {
        result = ma_sound_start(&currentSound);
        if (result != MA_SUCCESS)
        {
            isPlaying = false;
            std::cout << "Failed to start sound! Error code: " << result << std::endl;
            ma_sound_uninit(&currentSound);
            return;
        }
        ma_sound_set_volume(&currentSound, volume);
        isPlaying = true;
    }

    if (restartCurrentSong)
    {
        std::cout << "Restarting the current song." << std::endl;
    }
    else
    {
        std::cout << "Playing the previous song." << std::endl;
    }
}

void MusicPlayer::setVolume(float newVolume)
{
    volume = newVolume;
    ma_sound_set_volume(&currentSound, volume);
}

float MusicPlayer::getPlaybackPosition()
{
    float position = 0.0f;
    ma_sound_get_cursor_in_seconds(&currentSound, &position);
    return position;
}

void MusicPlayer::setPlaybackPosition(float position)
{
    ma_uint32 sampleRate = ma_engine_get_sample_rate(&engine);
    if (sampleRate == 0)
    {
        std::cout << "Invalid sample rate!";
        return;
    }

    ma_result result = ma_sound_seek_to_pcm_frame(&currentSound,
        static_cast<ma_uint64>(position * sampleRate));
    if (result != MA_SUCCESS)
    {
        std::cout << "Failed to seek to position! Error code:" << result;
        return;
    }
}

float MusicPlayer::getDuration()
{
    float duration = 0.0f;
    ma_sound_get_length_in_seconds(&currentSound, &duration);
    return duration;
}

void MusicPlayer::loadSongsFromDirectory(const std::string& directoryPath)
{
   for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
   {
       if (entry.is_regular_file())
       {
           std::string extension = entry.path().extension().string();
           if (extension == ".mp3" || extension == ".wav")
           {
               playlist.addToLibrary(Song(entry.path().string()));
           }
       }
   }
}

void MusicPlayer::displayShuffleControls()
{
    ImGui::Separator();
    if (ImGui::Button("Shuffle"))
    {
        if (playlist.getQueueSize() > 0)
        {
            const Song& currentSong = playlist.getCurrentSong(currentSongIndex);
            playlist.shuffleQueue(currentShuffleMode, currentSong, currentSongIndex);
        }
    }

    ImGui::SameLine();
    const char* modes[] = { "Random", "By Genre", "By Mood", "Weighted" };
    int currentMode = static_cast<int>(currentShuffleMode);
    if (ImGui::Combo("Shuffle Mode", &currentMode, modes, IM_ARRAYSIZE(modes)))
    {
        currentShuffleMode = static_cast<ShuffleMode>(currentMode);
    }
}
