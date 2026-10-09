---
title: "Music Playlist Simulator: Project Report"
---

# 1. Purpose of the Program

This program simulates the queue behind a music player. It holds a playlist of items (songs and podcast episodes), tracks which one is "currently playing," and lets the user skip forward, go back, add items, and remove items. Nothing is actually played. The program only prints text to the terminal, such as `Now playing: [Song] "Overclocked" by DryftiN (3:05)`.

The problem it models is a playlist that loops forever. When you skip past the last track, a real player wraps around to the first one, and when you go back from the first track it wraps to the last. The program solves this without any index math or "if at the end, reset to zero" logic by storing the items in a double-ended queue (`std::deque`). The first element of the deque is always the current item. Skipping moves the front item to the back, and going back moves the back item to the front. The program also demonstrates object-oriented design: an abstract base class (`MediaItem`) with two derived classes (`Song`, `Podcast`), a container class (`Playlist`), and a controller class (`MusicPlayer`).

# 2. How Users Interact With the Program

The program is a library plus a demo. There is no keyboard input: a user "interacts" by writing C++ code that creates objects and calls their methods, exactly as `src/main.cpp` does. Build and run the demo with:

    cmake -S . -B build && cmake --build build && ./build/playlist

## 2.1 Setting up

Include the headers, create a `Playlist`, and create a `MusicPlayer` that controls it (`src/main.cpp:3-6`, `10`, `17`):

    #include "MusicPlayer.h"
    #include "Playlist.h"
    #include "Podcast.h"
    #include "Song.h"

    Playlist playlist("cvnt");          // main.cpp:10
    MusicPlayer player(playlist);       // main.cpp:17

`MusicPlayer` holds a reference to the playlist (`include/MusicPlayer.h:17`), so the playlist must outlive the player.

## 2.2 Function reference

| What you want to do | Code | Declared | Implemented |
|---|---|---|---|
| Add an item to the end of the queue | `playlist.addToEnd(std::make_unique<Song>("Title", "Artist", seconds));` | `include/Playlist.h:26` | `src/Playlist.cpp:6-8` |
| Add an item to play right after the current one | `playlist.playNext(std::make_unique<Song>(...));` | `include/Playlist.h:27` | `src/Playlist.cpp:10-16` |
| Remove the current item | `playlist.removeCurrent();` (returns `false` if empty) | `include/Playlist.h:30` | `src/Playlist.cpp:18-22` |
| Remove the item at a position (0 = current) | `playlist.removeAt(2);` (returns `false` if out of range) | `include/Playlist.h:33` | `src/Playlist.cpp:24-28` |
| See what is current | `playlist.current();` (returns `nullptr` if empty) | `include/Playlist.h:36` | `src/Playlist.cpp:30-32` |
| Print the current item | `player.play();` | `include/MusicPlayer.h:10` | `src/MusicPlayer.cpp:13` |
| Skip to the next item | `player.skip();` (or `playlist.next()` without printing) | `include/MusicPlayer.h:11` | `src/MusicPlayer.cpp:14` |
| Go back to the previous item | `player.back();` (or `playlist.previous()` without printing) | `include/MusicPlayer.h:12` | `src/MusicPlayer.cpp:15` |
| Print the whole queue (`>` marks current) | `playlist.print();` | `include/Playlist.h:41` | `src/Playlist.cpp:48-53` |
| Get the number of items / check for empty | `playlist.size();` / `playlist.empty();` | `include/Playlist.h:23-24` | inline |

## 2.3 Adding items

Items are passed as `std::unique_ptr<MediaItem>`, because the playlist takes ownership. Use `std::make_unique` with either derived type (`src/main.cpp:11-16`):

    playlist.addToEnd(std::make_unique<Song>("Overclocked", "DryftiN", 185));
    playlist.addToEnd(std::make_unique<Podcast>("Some Title", "Host Name", 69, 1260));

The `Song` constructor takes (title, artist, duration in seconds). The `Podcast` constructor takes (title, host, episode number, duration in seconds). A song added with `playNext` (`src/main.cpp:30`) goes to position 1, directly behind the current item.

## 2.4 Removing items

Position numbers count in play order: position 0 is the current item, position 1 is next, and so on. The demo removes the item at position 2 and then the current item (`src/main.cpp:34-35`):

    playlist.removeAt(2);
    playlist.removeCurrent();

Worked example from the demo. Just before the removal, the playlist has 7 items (6 original + 1 added by `playNext` at `main.cpp:30`). Each removal deletes one item:

    7 items - removeAt(2) = 6 items
    6 items - removeCurrent() = 5 items

The demo's printed output confirms `Playlist "cvnt" (5 items)`. Both removal functions return a `bool` so calling code can tell whether anything was removed. An invalid index (for example `removeAt(99)`) or an empty playlist returns `false` and changes nothing.

## 2.5 Cycling

The demo skips forward 8 times through a 6-item playlist and then goes back 4 times (`src/main.cpp:24`, `27`). Because the list wraps, 8 skips on 6 items ends up at the same place as 8 mod 6 = 2 skips (8 / 6 = 1 remainder 2). The list is circular, so no extra code handles the wrap.

## 2.6 Empty playlist safety

Calling `play()` or `skip()` on an empty playlist does not crash. `Playlist::current()`, `next()` and `previous()` return `nullptr` when empty (`src/Playlist.cpp:31`, `35`, `42`), and `MusicPlayer::announce` prints "(playlist is empty)" when it receives `nullptr` (`src/MusicPlayer.cpp:6-9`). This is demonstrated at `src/main.cpp:38-43`.

# 3. How the Program Uses a Deque, the Standard Library, and Inheritance

## 3.1 The deque

A `std::deque` (double-ended queue) allows fast insertion and removal at both the front and the back. The playlist stores its items in one (`include/Playlist.h:45`):

    std::deque<std::unique_ptr<MediaItem>> items_;

The rule that makes it work: **the front of the deque is the current item.** Every operation follows from that rule.

| Operation | Deque calls | Location | Effect |
|---|---|---|---|
| `next()` | `push_back(std::move(front()))`, then `pop_front()` | `src/Playlist.cpp:36-37` | Current item goes to the back; the next item becomes the front |
| `previous()` | `push_front(std::move(back()))`, then `pop_back()` | `src/Playlist.cpp:43-44` | The last item becomes the front (current) |
| `addToEnd()` | `push_back` | `src/Playlist.cpp:7` | Item joins the end of the queue |
| `playNext()` | `insert(begin() + 1, ...)` | `src/Playlist.cpp:14` | Item goes right behind the current one |
| `removeCurrent()` | `pop_front` | `src/Playlist.cpp:20` | Current item is deleted |
| `removeAt(i)` | `erase(begin() + i)` | `src/Playlist.cpp:26` | Item at position `i` is deleted |
| `current()` | `front()` | `src/Playlist.cpp:31` | Reads the current item |

Worked example of `next()`. Start with a queue of [A, B, C], where A is current:

    Step 1: push_back(A)  ->  [A, B, C, A]
    Step 2: pop_front()   ->  [B, C, A]      B is now current

Worked example of `previous()` on [B, C, A]:

    Step 1: push_front(A) ->  [A, B, C, A]
    Step 2: pop_back()    ->  [A, B, C]      A is current again

**Why a deque and not a vector or list.** A `std::vector` is slow at removing from the front, because every other element shifts over by one. A `std::deque` does push and pop at both ends in constant time (O(1)), and `skip` and `back` are the two most common operations. Unlike `std::list`, a deque also supports random access, which `print()` (`src/Playlist.cpp:50-52`) and `removeAt` (`src/Playlist.cpp:26`) use through `items_[i]` and `begin() + i`. Note that `insert` and `erase` in the middle (`playNext` and `removeAt`) are O(n), because elements must shift. That is acceptable here because those are rare operations.

## 3.2 Other standard library features

| Feature | Where | Purpose |
|---|---|---|
| `std::deque` | `include/Playlist.h:4`, `45` | The queue itself |
| `std::unique_ptr` and `std::make_unique` | `include/Playlist.h:5`, `45`; `src/main.cpp:11-16` | The deque owns each item; memory is freed automatically when an item is removed or the playlist is destroyed (no `new`/`delete`) |
| `std::move` | `src/Playlist.cpp:7`, `12`, `14`, `36`, `43` | `unique_ptr` cannot be copied, only moved. Moving transfers ownership into or within the deque |
| `std::string` | `include/MediaItem.h:24`, `include/Song.h:15` | Titles, artists, hosts |
| `std::cout` | `src/Playlist.cpp:49-51`, `src/MusicPlayer.cpp:7`, `10` | All terminal output |
| `std::snprintf` | `src/MediaItem.cpp:7` | Formats seconds as `minutes:seconds` |
| `std::to_string` | `src/Podcast.cpp:4` | Converts the episode number to text |
| `std::size_t` | `include/Playlist.h:23`, `33` | Index and size type |

**Duration formatting example.** `formattedDuration()` (`src/MediaItem.cpp:5-9`) uses integer division and remainder. For a 185-second song:

    minutes = 185 / 60 = 3  (integer division; 3 x 60 = 180)
    seconds = 185 % 60 = 5  (185 - 180)
    printed with %d:%02d -> "3:05"

(`%02d` pads the seconds to two digits.) The formatter does not break out hours, so a very long item prints as a large minute count. For example, the 604800-second podcast in `src/main.cpp:15` prints as `10080:00`, since 604800 / 60 = 10080 minutes with remainder 0.

## 3.3 Inheritance and polymorphism

**Class hierarchy**

    MediaItem  (abstract base class)
      |-- Song
      |-- Podcast

- **`MediaItem`** (`include/MediaItem.h:7-26`) is an abstract base class. It stores what every item has in common: a title and a duration (`24-25`). It declares `virtual std::string describe() const = 0;` (`17`), a pure virtual function, so `MediaItem` itself cannot be instantiated. Each concrete type must say how it describes itself. It also provides a `protected` helper, `formattedDuration()` (`21`), so both subclasses format time identically without duplicating code, and so outside code cannot call it directly.
- **`Song`** (`include/Song.h:5`) is declared `class Song : public MediaItem`. It adds an `artist_` field (`15`) and overrides `describe()` with the `override` keyword (`12`; implementation at `src/Song.cpp:3-5`).
- **`Podcast`** (`include/Podcast.h:5`) is declared `class Podcast : public MediaItem`. It adds `host_` and `episode_` fields (`14-15`) and overrides `describe()` (`11`; implementation at `src/Podcast.cpp:3-6`).
- The derived constructors call the base constructor in their initializer lists to set the shared title and duration: `MediaItem(std::move(title), durationSeconds)` (`include/Song.h:8`, `include/Podcast.h:8`).

**Polymorphism in action.** The deque holds `std::unique_ptr<MediaItem>`, a pointer to the base type, so one container can store both songs and podcasts (`include/Playlist.h:45`). When `Playlist::print()` calls `items_[i]->describe()` (`src/Playlist.cpp:51`) and when `MusicPlayer::announce` calls `item->describe()` (`src/MusicPlayer.cpp:10`), C++ picks `Song::describe` or `Podcast::describe` at runtime depending on the actual object. Neither `Playlist` nor `MusicPlayer` knows or cares which kinds of items exist. A new type such as `Audiobook` could be added by inheriting from `MediaItem` and overriding `describe()`, with no changes to either of them.

**Virtual destructor.** `MediaItem` declares `virtual ~MediaItem() = default;` (`include/MediaItem.h:11`). Without it, deleting a `Song` through a `MediaItem` pointer (which is what `unique_ptr<MediaItem>` does) would be undefined behavior and the derived part would not be cleaned up properly.

## 3.4 Other object-oriented practices

- **Encapsulation:** all data members are `private` (`include/Playlist.h:44-45`, `include/MediaItem.h:24-25`). The deque is never exposed. Outside code can only change the queue through the methods listed in section 2.2.
- **Composition:** a `Playlist` *has* a deque of items; a `MusicPlayer` *has* a reference to a `Playlist` (`include/MusicPlayer.h:17`). These are "has-a" relationships, in contrast with the "is-a" relationships of inheritance (`Song` *is a* `MediaItem`).
- **Single responsibility:** `MediaItem` and its subclasses describe content; `Playlist` manages the queue; `MusicPlayer` handles playback messages.
- **Const-correctness:** methods that do not change state are marked `const` (for example `describe()` at `include/MediaItem.h:17`, `current()` at `include/Playlist.h:36`, `print()` at `include/Playlist.h:41`).
- **Ownership (RAII):** `unique_ptr` ties each item's lifetime to the deque, so removing an item (`src/Playlist.cpp:20`, `26`) automatically frees it.

# 4. Files in the Project

| File | Contents |
|---|---|
| `include/MediaItem.h`, `src/MediaItem.cpp` | Abstract base class |
| `include/Song.h`, `src/Song.cpp` | Derived class |
| `include/Podcast.h`, `src/Podcast.cpp` | Derived class |
| `include/Playlist.h`, `src/Playlist.cpp` | Deque-backed queue |
| `include/MusicPlayer.h`, `src/MusicPlayer.cpp` | Playback controller |
| `src/main.cpp` | Demonstration program |
| `CMakeLists.txt` | Build configuration (C++17) |

*Line numbers refer to the code as of the commit that added `removeCurrent()` and `removeAt()`.*
