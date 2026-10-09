#include <iostream>
#include <memory>

#include "MusicPlayer.h"
#include "Playlist.h"
#include "Podcast.h"
#include "Song.h"

int main() {
    Playlist playlist("Study Mix");
    playlist.addToEnd(std::make_unique<Song>("Clair de Lune", "Debussy", 305));
    playlist.addToEnd(std::make_unique<Song>("Weightless", "Marconi Union", 485));
    playlist.addToEnd(std::make_unique<Podcast>("Intro to Deques", "Dana Lee", 12, 1260));
    playlist.addToEnd(std::make_unique<Song>("Gymnopedie No. 1", "Satie", 190));

    MusicPlayer player(playlist);

    std::cout << "=== Initial state ===\n";
    playlist.print();
    player.play();

    std::cout << "\n=== Skip forward 5 times (wraps around the 4-item list) ===\n";
    for (int i = 0; i < 5; ++i) player.skip();

    std::cout << "\n=== Go back twice ===\n";
    player.back();
    player.back();

    std::cout << "\n=== Queue a song to play next ===\n";
    playlist.playNext(std::make_unique<Song>("Nocturne Op. 9 No. 2", "Chopin", 270));
    playlist.print();
    player.skip();

    std::cout << "\n=== Empty playlist is handled safely ===\n";
    Playlist empty("Empty");
    MusicPlayer emptyPlayer(empty);
    emptyPlayer.play();
    emptyPlayer.skip();
    return 0;
}
