cmake_minimum_required(VERSION 3.10)
project(MusicPlaylist CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(playlist
    src/main.cpp src/MediaItem.cpp src/Song.cpp src/Podcast.cpp
    src/Playlist.cpp src/MusicPlayer.cpp)
target_include_directories(playlist PRIVATE include)
target_compile_options(playlist PRIVATE -Wall -Wextra -pedantic)
