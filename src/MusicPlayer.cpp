#include "MusicPlayer.h"

#include <iostream>

void MusicPlayer::announce(const char* action, const MediaItem* item) const {
    if (!item) {
        std::cout << "(playlist is empty)\n";
        return;
    }
    std::cout << action << ": " << item->describe() << '\n';
}

void MusicPlayer::play() const { announce("Now playing", playlist_.current()); }
void MusicPlayer::skip()       { announce("Skipped to", playlist_.next()); }
void MusicPlayer::back()       { announce("Went back to", playlist_.previous()); }
