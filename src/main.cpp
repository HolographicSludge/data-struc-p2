#include <iostream>
#include <memory>
#include "MusicPlayer.h"
#include "Playlist.h"
#include "Podcast.h"
#include "Song.h"

int main() {
    std::cout << "Begin example demonstration\n";
    Playlist playlist("cvnt");
    playlist.addToEnd(std::make_unique<Song>("Overclocked", "DryftiN", 185));
    playlist.addToEnd(std::make_unique<Song>("American Idiot", "Green Day", 174));
    playlist.addToEnd(std::make_unique<Podcast>("How to Not Give a Fuck", "GuyOnTumblr", 69, 1260));
    playlist.addToEnd(std::make_unique<Song>("Young Girl A", "Siinamota", 242));
    playlist.addToEnd(std::make_unique<Podcast>("Idk I Don't Listen to Podcasts", "Joe Rogan's Gay Boyfriend", 12, 604800));
    playlist.addToEnd(std::make_unique<Song>("PUPPYPLAY", "MAILPUP", 172));
    MusicPlayer player(playlist);

    std::cout << "\nStarting state of playlist:\n";
    playlist.print();
    player.play();

    std::cout << "\nSkip forward 8x (wraps around the 6-item example playlist)\n";
    for (int i = 0; i < 8; i++) player.skip();

    std::cout << "\nGo back 4x\n";
    for (int i = 0; i < 4; i++) player.back();

    std::cout << "\nQueue another song to play next\n";
    playlist.playNext(std::make_unique<Song>("Ego Renegade Boy", "FLAVOR FOLEY", 199));
    playlist.print();

    std::cout << "\nRemove the item at position 2, then remove the current item\n";
    playlist.removeAt(2);
    playlist.removeCurrent();
    playlist.print();

    std::cout << "\nDemonstration 2: empty playlist is handled safely\n";
    Playlist empty("Empty");
    MusicPlayer emptyPlayer(empty);
    emptyPlayer.play();
    emptyPlayer.skip();
    std::cout << "If you're seeing this line, no errors were thrown\n";

    return 0;
}
