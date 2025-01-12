// song.h
#pragma once
#include <string>
#include <array>
#include <taglib/fileref.h>
#include <taglib/tag.h>

enum class Genre
{
    None,
    Rock,
    Pop,
    Jazz,
    Classical,
    Electronic,
    HipHop,
    Metal,
    Folk,
    Blues,
    Reggae,
    Country,
    Soul,
    Funk,
    Punk,
    Indie,
    Alternative,
    Disco,
    House,
    Techno,
    Trance,
    Dubstep,
    Ambient,
    World,
    Latin,
    Gospel,
    Swing,
    Opera,
    KPop,
    JPop,
    Synthwave,
    Experimental,
    Grunge,
    NewAge,
    COUNT
};

enum class Mood
{
    None,
    Energetic,
    Calm,
    Melancholic,
    Happy,
    Aggressive,
    Romantic,
    Dark,
    Ethereal,
    Joyful,
    Reflective,
    Hopeful,
    Nostalgic,
    Mysterious,
    Tense,
    Sad,
    Uplifting,
    Playful,
    Somber,
    Inspiring,
    Dreamy,
    Adventurous,
    Angry,
    Peaceful,
    Triumphant,
    Serene,
    Bittersweet,
    Suspenseful,
    Exciting,
    Chill,
    Haunting,
    COUNT
};

std::string genreToString(Genre inGenre);
std::string moodToString(Mood inMood);

class Song
{
public:
    Song(const std::string& path);

    std::string getTitle() const;
    std::string getArtist() const;
    std::string getAlbum() const;
    std::string getFilePath() const;
    
    
    unsigned int getYear() const;
    
    void setGenres(const std::array<Genre, 3>& genres);
    void setMoods(const std::array<Mood, 3>& moods);
    const std::array<Genre, 3>& getGenres() const;
    const std::array<Mood, 3>& getMoods() const;

private:
    std::string filepath;
    std::array<Genre, 3> genres;
    std::array<Mood, 3> moods;
    std::string title;
    std::string artist;
    std::string album;
    unsigned int year;
};