#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <string>

#include "MediaItem.h"

// Owns a std::deque of MediaItems. The FRONT of the deque is the current item.
//   next()      : pop_front -> push_back   (current goes to the back)
//   previous()  : pop_back  -> push_front  (last item becomes current)
//   addToEnd()  : push_back
//   playNext()  : insert right after the current item
//   removeCurrent() : pop_front;  removeAt(i) : erase(begin() + i)
// Because both ends are O(1), cycling forward and backward is cheap and the
// playlist loops forever with no index bookkeeping.
class Playlist {
public:
    explicit Playlist(std::string name) : name_(std::move(name)) {}

    const std::string& name() const { return name_; }
    std::size_t size() const { return items_.size(); }
    bool empty() const { return items_.empty(); }

    void addToEnd(std::unique_ptr<MediaItem> item);
    void playNext(std::unique_ptr<MediaItem> item);

    // Remove the current item (index 0). Returns false if the playlist is empty.
    bool removeCurrent();
    // Remove the item at a position in play order (0 = current).
    // Returns false if the index is out of range.
    bool removeAt(std::size_t index);

    // Both return nullptr if the playlist is empty.
    const MediaItem* current() const;
    const MediaItem* next();
    const MediaItem* previous();

    // Prints the whole queue in play order, marking the current item.
    void print() const;

private:
    std::string name_;
    std::deque<std::unique_ptr<MediaItem>> items_;
};
