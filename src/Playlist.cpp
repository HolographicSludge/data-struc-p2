#include "Playlist.h"

#include <iostream>
#include <utility>

void Playlist::addToEnd(std::unique_ptr<MediaItem> item) {
    items_.push_back(std::move(item));
}

void Playlist::playNext(std::unique_ptr<MediaItem> item) {
    if (items_.empty()) {
        items_.push_back(std::move(item));
    } else {
        items_.insert(items_.begin() + 1, std::move(item));
    }
}

const MediaItem* Playlist::current() const {
    return items_.empty() ? nullptr : items_.front().get();
}

const MediaItem* Playlist::next() {
    if (items_.empty()) return nullptr;
    items_.push_back(std::move(items_.front()));
    items_.pop_front();
    return items_.front().get();
}

const MediaItem* Playlist::previous() {
    if (items_.empty()) return nullptr;
    items_.push_front(std::move(items_.back()));
    items_.pop_back();
    return items_.front().get();
}

void Playlist::print() const {
    std::cout << "Playlist \"" << name_ << "\" (" << items_.size() << " items):\n";
    for (std::size_t i = 0; i < items_.size(); ++i) {
        std::cout << (i == 0 ? "  > " : "    ") << items_[i]->describe() << '\n';
    }
}
