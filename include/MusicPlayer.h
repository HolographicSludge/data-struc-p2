#pragma once

#include "Playlist.h"

// Controller: "plays" the current item of a playlist by printing text.
class MusicPlayer {
public:
    explicit MusicPlayer(Playlist& playlist) : playlist_(playlist) {}

    void play() const;
    void skip();
    void back();

private:
    void announce(const char* action, const MediaItem* item) const;

    Playlist& playlist_;  // not owned
};
