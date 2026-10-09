# Music Playlist (C++17, `std::deque`)

A terminal-only demo that cycles through a playlist. Nothing is played; it prints text.

## Build and run
    cmake -S . -B build && cmake --build build && ./build/playlist

## Classes
| Class | Role |
|---|---|
| `MediaItem` | Abstract base class (title, duration, pure virtual `describe()`) |
| `Song`, `Podcast` | Derived classes; override `describe()` |
| `Playlist` | Owns a `std::deque<std::unique_ptr<MediaItem>>`; front = current item |
| `MusicPlayer` | Controller that plays/skips/goes back on a `Playlist` |

## How the deque is used
- `next()`: `push_back(front)` then `pop_front()` (current item moves to the back)
- `previous()`: `push_front(back)` then `pop_back()`
- `addToEnd()`: `push_back`; `playNext()`: insert at position 1
- Both ends are O(1), so the playlist loops forever without index math.

## OOP concepts shown
- **Inheritance + polymorphism**: `Song`/`Podcast` derive from `MediaItem`, called through base pointers
- **Abstraction**: `MediaItem` cannot be instantiated; `describe()` is the interface
- **Encapsulation**: data members are private; the deque is never exposed
- **Ownership (RAII)**: `unique_ptr` owns items; virtual destructor on the base
- **Composition**: `Playlist` has items; `MusicPlayer` references a `Playlist`
