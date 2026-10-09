#pragma once

#include "MediaItem.h"

class Song : public MediaItem {
public:
    Song(std::string title, std::string artist, int durationSeconds)
        : MediaItem(std::move(title), durationSeconds), artist_(std::move(artist)) {}

    const std::string& artist() const { return artist_; }

    std::string describe() const override;

private:
    std::string artist_;
};
