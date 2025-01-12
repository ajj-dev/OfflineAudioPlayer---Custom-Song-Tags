#include "Song.h"

Song::Song(const std::string& filepath) : filepath(filepath)
{
    genres.fill(Genre::None);
    moods.fill(Mood::None);

    TagLib::FileRef f(filepath.c_str());
    if (!f.isNull() && f.tag())
    {
        TagLib::Tag* tag = f.tag();
        title = tag->title().isEmpty() ? "Unknown" : tag->title().toCString();
        artist = tag->artist().isEmpty() ? "Unknown" : tag->artist().toCString();
        album = tag->album().isEmpty() ? "Unknown" : tag->album().toCString();
    }
    else
    {
        title = "Unknown";
        artist = "Unknown";
        album = "Unknown";
    }
}

std::string genreToString(Genre inGenre) 
{
    switch (inGenre)
    {
    case Genre::None: return "None";
    case Genre::Rock: return "Rock";
    case Genre::Pop: return "Pop";
    case Genre::Jazz: return "Jazz";
    case Genre::Classical: return "Classical";
    case Genre::Electronic: return "Electronic";
    case Genre::HipHop: return "Hip-Hop";
    case Genre::Metal: return "Metal";
    case Genre::Folk: return "Folk";
    case Genre::Blues: return "Blues";
    case Genre::Reggae: return "Reggae";
    case Genre::Country: return "Country";
    case Genre::Soul: return "Soul";
    case Genre::Funk: return "Funk";
    case Genre::Punk: return "Punk";
    case Genre::Indie: return "Indie";
    case Genre::Alternative: return "Alternative";
    case Genre::Disco: return "Disco";
    case Genre::House: return "House";
    case Genre::Techno: return "Techno";
    case Genre::Trance: return "Trance";
    case Genre::Dubstep: return "Dubstep";
    case Genre::Ambient: return "Ambient";
    case Genre::World: return "World";
    case Genre::Latin: return "Latin";
    case Genre::Gospel: return "Gospel";
    case Genre::Swing: return "Swing";
    case Genre::Opera: return "Opera";
    case Genre::KPop: return "K-Pop";
    case Genre::JPop: return "J-Pop";
    case Genre::Synthwave: return "Synthwave";
    case Genre::Experimental: return "Experimental";
    case Genre::Grunge: return "Grunge";
    case Genre::NewAge: return "New Age";
    default: return "Unknown";
    }
}

std::string moodToString(Mood inMood)
{
    switch (inMood)
    {
    case Mood::None: return "None";
    case Mood::Energetic: return "Energetic";
    case Mood::Calm: return "Calm";
    case Mood::Melancholic: return "Melancholic";
    case Mood::Happy: return "Happy";
    case Mood::Aggressive: return "Aggressive";
    case Mood::Romantic: return "Romantic";
    case Mood::Dark: return "Dark";
    case Mood::Ethereal: return "Ethereal";
    case Mood::Joyful: return "Joyful";
    case Mood::Reflective: return "Reflective";
    case Mood::Hopeful: return "Hopeful";
    case Mood::Nostalgic: return "Nostalgic";
    case Mood::Mysterious: return "Mysterious";
    case Mood::Tense: return "Tense";
    case Mood::Sad: return "Sad";
    case Mood::Uplifting: return "Uplifting";
    case Mood::Playful: return "Playful";
    case Mood::Somber: return "Somber";
    case Mood::Inspiring: return "Inspiring";
    case Mood::Dreamy: return "Dreamy";
    case Mood::Adventurous: return "Adventurous";
    case Mood::Angry: return "Angry";
    case Mood::Peaceful: return "Peaceful";
    case Mood::Triumphant: return "Triumphant";
    case Mood::Serene: return "Serene";
    case Mood::Bittersweet: return "Bittersweet";
    case Mood::Suspenseful: return "Suspenseful";
    case Mood::Exciting: return "Exciting";
    case Mood::Chill: return "Chill";
    case Mood::Haunting: return "Haunting";
    default: return "Unknown";
    }
}

std::string Song::getArtist() const
{
    return artist;
}

std::string Song::getTitle() const
{
    return title;
}

std::string Song::getFilePath() const
{
    return filepath;
}

std::string Song::getAlbum() const
{
    return album;
}

unsigned int Song::getYear() const
{
    return year;
}

void Song::setGenres(const std::array<Genre, 3>& newGenres)
{
    genres = newGenres;
}

void Song::setMoods(const std::array<Mood, 3>& newMoods)
{
    moods = newMoods;
}

const std::array<Genre, 3>& Song::getGenres() const
{
    return genres;
}

const std::array<Mood, 3>& Song::getMoods() const
{
    return moods;
}
