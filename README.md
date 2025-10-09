# Offline Audio Player - Custom Song Tags

A C++ desktop audio player with custom genre and mood tagging capabilities. Built for offline music libraries with an intuitive GUI and smart shuffle features.

## Features

- **Custom Tagging System**: Tag songs with up to 3 genres and 3 moods each
- **Smart Shuffle Modes**:
  - Random shuffle
  - Genre-based shuffle
  - Mood-based shuffle
  - Weighted shuffle (considers both genre and mood)
- **Clean Modern UI**: Dark-themed interface built with ImGui
- **Playback Controls**: Play, pause, previous, next with seek functionality
- **Queue Management**: Paginated song queue with easy navigation
- **Persistent Tags**: Song tags are saved to JSON and persist between sessions
- **Metadata Support**: Automatically reads song metadata using TagLib

## Dependencies

This project requires the following libraries:

- **miniaudio** (included) - Audio playback engine
- **ImGui** - GUI framework
- **GLFW** - Window and input handling
- **OpenGL** - Graphics rendering
- **TagLib** - Audio metadata reading
- **nlohmann/json** - JSON parsing for tag storage

## Project Structure

```
offlineAudioPlayer/
├── main.cpp              # Application entry point and ImGui setup
├── MusicPlayer.cpp/h     # Core player logic and UI rendering
├── Song.cpp/h            # Song class with genre/mood definitions
├── SongList.cpp/h        # Playlist management and shuffle algorithms
├── miniaudio.h           # Single-header audio library
├── songLibrary/          # Place your MP3/WAV files here
└── songTags.json         # Auto-generated tag storage (created on first run)
```

## Building

Ensure you have the following installed:
- C++17 or later compiler
- GLFW3
- OpenGL
- TagLib
- nlohmann/json
- ImGui with GLFW and OpenGL3 backends

### Installing Dependencies

Using vcpkg (recommended):
```bash
vcpkg install imgui[core,glfw-binding,opengl3-binding] glfw3 taglib nlohmann-json
```

Or install manually through your system's package manager.

### Compile and Run

```bash
# Compile (adjust flags/paths as needed for your system)
g++ -std=c++17 main.cpp MusicPlayer.cpp Song.cpp SongList.cpp \
    -lglfw -lGL -ltag -lpthread -ldl \
    -I/path/to/imgui -I/path/to/imgui/backends \
    /path/to/imgui/imgui*.cpp /path/to/imgui/backends/imgui_impl_glfw.cpp \
    /path/to/imgui/backends/imgui_impl_opengl3.cpp \
    -o audioPlayer

# Run
./audioPlayer
```

## Usage

1. Place your audio files (MP3, WAV) in the `songLibrary/` directory
2. Run the application
3. Songs will automatically appear in the queue
4. Right-click any song to edit its genres and moods
5. Use the shuffle dropdown to choose a shuffle mode
6. Tags are automatically saved and will persist when you reopen the app

## Shuffle Modes

- **Random**: Completely random shuffle
- **By Genre**: Prioritizes songs with similar genres to the current track
- **By Mood**: Prioritizes songs with similar moods to the current track
- **Weighted**: Considers both genre and mood similarity

## Supported Tags

### Genres (33 total)
Rock, Pop, Jazz, Classical, Electronic, Hip-Hop, Metal, Folk, Blues, Reggae, Country, Soul, Funk, Punk, Indie, Alternative, Disco, House, Techno, Trance, Dubstep, Ambient, World, Latin, Gospel, Swing, Opera, K-Pop, J-Pop, Synthwave, Experimental, Grunge, New Age

### Moods (30 total)
Energetic, Calm, Melancholic, Happy, Aggressive, Romantic, Dark, Ethereal, Joyful, Reflective, Hopeful, Nostalgic, Mysterious, Tense, Sad, Uplifting, Playful, Somber, Inspiring, Dreamy, Adventurous, Angry, Peaceful, Triumphant, Serene, Bittersweet, Suspenseful, Exciting, Chill, Haunting

## License

[Add your license here]

## Contributing

Contributions are welcome! Feel free to submit issues or pull requests.

## Acknowledgments

- [miniaudio](https://miniaud.io/) - David Reid
- [Dear ImGui](https://github.com/ocornut/imgui) - Omar Cornut
- [TagLib](https://taglib.org/) - TagLib team
- [nlohmann/json](https://github.com/nlohmann/json) - Niels Lohmann
